#include "SerialPort.hpp"

// SetupAPI for port enumeration
#include <setupapi.h>
#include <devguid.h> // GUID_DEVCLASS_PORTS
// #pragma comment(lib, "setupapi.lib")

#include <algorithm>
#include <cassert>
#include <chrono>
#include <utility> // std::exchange, std::to_underlying

// ================================================================
//  Move semantics
//  NOTE: after a move the moved-from object is closed/empty.
//  Do NOT move a port while its threads are running — always
//  close() first.
// ================================================================

SerialPort::SerialPort(SerialPort &&o) noexcept { *this = std::move(o); }

SerialPort &SerialPort::operator=(SerialPort &&o) noexcept
{
    if (this == &o)
        return *this;
    close();
    m_handle = std::exchange(o.m_handle, INVALID_HANDLE_VALUE);
    m_stopEvent = std::exchange(o.m_stopEvent, nullptr);
    m_portName = std::move(o.m_portName);
    m_config = o.m_config;
    // Callbacks are intentionally not transferred here so the
    // caller can re-hook them after the move if desired.
    return *this;
}

// ================================================================
//  Port enumeration  (SetupAPI — finds all present COM ports)
// ================================================================

std::vector<std::string> SerialPort::enumeratePorts()
{
    std::vector<std::string> result;

    HDEVINFO hDev = SetupDiGetClassDevsA(
        &GUID_DEVCLASS_PORTS,
        nullptr, nullptr,
        DIGCF_PRESENT);

    if (hDev == INVALID_HANDLE_VALUE)
        return result;

    SP_DEVINFO_DATA devInfo{};
    devInfo.cbSize = sizeof(devInfo);

    for (DWORD idx = 0; SetupDiEnumDeviceInfo(hDev, idx, &devInfo); ++idx)
    {
        HKEY hKey = SetupDiOpenDevRegKey(
            hDev, &devInfo,
            DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

        if (hKey == INVALID_HANDLE_VALUE)
            continue;

        char portName[32]{};
        DWORD sz = sizeof(portName);
        DWORD type = 0;

        LSTATUS status = RegQueryValueExA(
            hKey, "PortName", nullptr, &type,
            reinterpret_cast<BYTE *>(portName), &sz);

        if (status == ERROR_SUCCESS &&
            std::string_view(portName).starts_with("COM"))
        {
            result.emplace_back(portName);
        }

        RegCloseKey(hKey);
    }

    SetupDiDestroyDeviceInfoList(hDev);

    // Sort numerically: COM1, COM2, …, COM10, COM11, …
    std::sort(result.begin(), result.end(),
              [](const std::string &a, const std::string &b)
              {
                  int na = std::stoi(a.substr(3));
                  int nb = std::stoi(b.substr(3));
                  return na < nb;
              });

    return result;
}

// ================================================================
//  open
// ================================================================

bool SerialPort::open(const std::string &portName, const SerialConfig &cfg)
{
    close();

    // Paths like \\.\COM10 work for any port number
    const std::string path = "\\\\.\\" + portName;

    m_handle = CreateFileA(
        path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,       // exclusive access
        nullptr, // no security attrs
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        nullptr);

    if (m_handle == INVALID_HANDLE_VALUE)
        return false;

    m_portName = portName;
    m_config = cfg;

    if (!applyConfig(cfg))
    {
        CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
        return false;
    }

    // Shared manual-reset stop event used by both worker threads
    m_stopEvent = CreateEventA(nullptr, /*bManualReset=*/TRUE, /*bInit=*/FALSE, nullptr);
    if (!m_stopEvent)
    {
        CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
        return false;
    }

    m_running = true;
    startMonitorThread();
    startWriteThread();
    return true;
}

// ================================================================
//  close
// ================================================================

void SerialPort::close()
{
    if (!isOpen())
        return;

    // 1. Signal both threads to stop
    m_running = false;
    SetEvent(m_stopEvent);       // wakes monitor thread
    m_writeQueueCV.notify_all(); // wakes write thread
    m_rxCV.notify_all();         // wakes any readBlocking() callers

    // 2. Join threads before touching handles
    stopMonitorThread();
    stopWriteThread();

    // 3. Release Win32 resources
    CloseHandle(m_handle);
    m_handle = INVALID_HANDLE_VALUE;
    CloseHandle(m_stopEvent);
    m_stopEvent = nullptr;

    // 4. Clear state
    m_portName.clear();
    {
        std::lock_guard lk(m_rxMutex);
        m_rxBuffer.clear();
    }
    {
        std::lock_guard lk(m_writeQueueMutex);
        while (!m_writeQueue.empty())
            m_writeQueue.pop();
    }
}

bool SerialPort::isOpen() const noexcept
{
    return m_handle != INVALID_HANDLE_VALUE;
}

// ================================================================
//  applyConfig  —  DCB + COMMTIMEOUTS + SetupComm
//  std::to_underlying converts each scoped enum to its Win32 type.
// ================================================================

bool SerialPort::applyConfig(const SerialConfig &cfg)
{
    // ---- DCB ------------------------------------------------
    DCB dcb{};
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(m_handle, &dcb))
        return false;

    // std::to_underlying used for every scoped-enum → Win32 conversion
    dcb.BaudRate = std::to_underlying(cfg.baudRate);
    dcb.ByteSize = std::to_underlying(cfg.dataBits);
    dcb.StopBits = std::to_underlying(cfg.stopBits);
    dcb.Parity = std::to_underlying(cfg.parity);
    dcb.fParity = (cfg.parity != Parity::None) ? TRUE : FALSE;
    dcb.fBinary = TRUE; // required for serial comm

    // Default: no flow control
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;

    switch (cfg.flowControl)
    {
        using enum FlowControl; // C++20 using-enum

    case Hardware:
        dcb.fOutxCtsFlow = TRUE;
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
        break;

    case Software:
        dcb.fOutX = TRUE;
        dcb.fInX = TRUE;
        dcb.XonChar = 0x11;  // ^Q
        dcb.XoffChar = 0x13; // ^S
        dcb.XonLim = 100;
        dcb.XoffLim = 100;
        break;

    case None:
    default:
        break;
    }

    if (!SetCommState(m_handle, &dcb))
        return false;

    // ---- COMMTIMEOUTS ----------------------------------------
    // ReadInterval = MAXDWORD + ReadTotal = 0 → return immediately
    // with whatever bytes are in the hardware buffer.  The monitor
    // thread drives all async reads; these timeouts only affect the
    // synchronous ReadFile calls inside drainHardwareBuffer().
    COMMTIMEOUTS to{};
    to.ReadIntervalTimeout = MAXDWORD;
    to.ReadTotalTimeoutMultiplier = 0;
    to.ReadTotalTimeoutConstant = 0;
    to.WriteTotalTimeoutMultiplier = 0;
    to.WriteTotalTimeoutConstant = cfg.writeTimeoutMs;

    if (!SetCommTimeouts(m_handle, &to))
        return false;

    // ---- Driver buffers --------------------------------------
    SetupComm(m_handle, cfg.rxDriverBuf, cfg.txDriverBuf);
    return true;
}

// ================================================================
//  Monitor thread  —  watches comm events via overlapped WaitCommEvent
// ================================================================

void SerialPort::startMonitorThread()
{
    m_monitorThread = std::thread(&SerialPort::monitorThreadFunc, this);
}

void SerialPort::stopMonitorThread()
{
    if (m_monitorThread.joinable())
        m_monitorThread.join();
}

void SerialPort::monitorThreadFunc()
{
    // Subscribe to every event we surface through PortEvent
    constexpr DWORD kMask =
        EV_RXCHAR | // byte(s) received
        EV_ERR |    // line-status error
        EV_BREAK |  // break condition
        EV_RING |   // ring indicator
        EV_RLSD |   // carrier detect changed
        EV_CTS |    // CTS changed
        EV_DSR;     // DSR changed

    SetCommMask(m_handle, kMask);

    OVERLAPPED ovWait{};
    ovWait.hEvent = CreateEventA(nullptr, /*bManualReset=*/TRUE, FALSE, nullptr);
    if (!ovWait.hEvent)
        return;

    // We wait on two handles: the stop signal and the comm event
    const HANDLE waitSet[2] = {m_stopEvent, ovWait.hEvent};

    while (m_running)
    {
        ResetEvent(ovWait.hEvent);
        DWORD evtMask = 0;

        BOOL ok = WaitCommEvent(m_handle, &evtMask, &ovWait);

        if (!ok)
        {
            const DWORD err = GetLastError();

            if (err == ERROR_OPERATION_ABORTED)
                break; // port is closing

            if (err != ERROR_IO_PENDING)
                break; // unexpected error

            // Pending — wait for comm event OR stop signal
            DWORD wr = WaitForMultipleObjects(2, waitSet, FALSE, INFINITE);

            if (wr == WAIT_OBJECT_0)
                break; // stop event fired

            DWORD transferred = 0;
            if (!GetOverlappedResult(m_handle, &ovWait, &transferred, FALSE))
            {
                if (GetLastError() == ERROR_OPERATION_ABORTED)
                    break;
                continue; // transient error; retry
            }
        }

        if (!m_running)
            break;

        // ---- RX data ----------------------------------------
        if (evtMask & EV_RXCHAR)
        {
            std::string chunk = drainHardwareBuffer();

            if (!chunk.empty())
            {
                // 1. Append to internal buffer and wake readBlocking()
                {
                    std::lock_guard lk(m_rxMutex);
                    m_rxBuffer += chunk;
                }
                m_rxCV.notify_all();

                // 2. Fire the read callback (if registered)
                {
                    std::lock_guard lk(m_callbackMutex);
                    if (m_readCallback)
                        m_readCallback(chunk);
                }
            }
        }

        // ---- Modem / line events ----------------------------
        {
            std::lock_guard lk(m_callbackMutex);
            if (m_eventCallback)
            {
                using enum PortEvent;
                if (evtMask & EV_ERR)
                    m_eventCallback(LineError);
                if (evtMask & EV_BREAK)
                    m_eventCallback(BreakDetected);
                if (evtMask & EV_RING)
                    m_eventCallback(RingDetected);
                if (evtMask & EV_RLSD)
                    m_eventCallback(CarrierChanged);
                if (evtMask & EV_CTS)
                    m_eventCallback(CTSChanged);
                if (evtMask & EV_DSR)
                    m_eventCallback(DSRChanged);
            }
        }
    }

    CloseHandle(ovWait.hEvent);
}

// Drains every byte currently sitting in the hardware RX FIFO.
// Called exclusively from the monitor thread.
std::string SerialPort::drainHardwareBuffer()
{
    DWORD errors = 0;
    COMSTAT cs{};
    if (!ClearCommError(m_handle, &errors, &cs) || cs.cbInQue == 0)
        return {};

    std::string buf(cs.cbInQue, '\0');
    DWORD bytesRead = 0;

    OVERLAPPED ovRead{};
    ovRead.hEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);

    BOOL ok = ReadFile(
        m_handle, buf.data(), cs.cbInQue, &bytesRead, &ovRead);

    if (!ok)
    {
        if (GetLastError() == ERROR_IO_PENDING)
            GetOverlappedResult(m_handle, &ovRead, &bytesRead, /*bWait=*/TRUE);
        else
            bytesRead = 0;
    }

    CloseHandle(ovRead.hEvent);
    buf.resize(bytesRead);
    return buf;
}

// ================================================================
//  Write thread  —  serialises writes from the queue
// ================================================================

void SerialPort::startWriteThread()
{
    m_writeThread = std::thread(&SerialPort::writeThreadFunc, this);
}

void SerialPort::stopWriteThread()
{
    // m_writeQueueCV is already notified in close(); just join.
    if (m_writeThread.joinable())
        m_writeThread.join();
}

void SerialPort::writeThreadFunc()
{
    while (true)
    {
        WriteOp op;
        {
            std::unique_lock lk(m_writeQueueMutex);
            m_writeQueueCV.wait(lk, [this]
                                { return !m_writeQueue.empty() || !m_running; });

            if (!m_running && m_writeQueue.empty())
                break;
            if (m_writeQueue.empty())
                continue;

            op = std::move(m_writeQueue.front());
            m_writeQueue.pop();
        }

        const bool result = doWrite(op.data, op.timeoutMs);

        if (op.pPromise)
            op.pPromise->set_value(result);
    }
}

// Core overlapped WriteFile — called exclusively by the write thread.
bool SerialPort::doWrite(const std::string &data, DWORD timeoutMs)
{
    if (data.empty())
        return true;
    if (!isOpen())
        return false;

    OVERLAPPED ov{};
    ov.hEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);
    if (!ov.hEvent)
        return false;

    DWORD written = 0;
    BOOL ok = WriteFile(
        m_handle,
        data.data(),
        static_cast<DWORD>(data.size()),
        &written,
        &ov);

    if (!ok)
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {
            CloseHandle(ov.hEvent);
            return false;
        }

        // Wait on either write completion or stop signal
        const HANDLE waitSet[2] = {m_stopEvent, ov.hEvent};
        DWORD wr = WaitForMultipleObjects(2, waitSet, FALSE, timeoutMs);

        if (wr != WAIT_OBJECT_0 + 1)
        { // not the write-complete event
            CancelIo(m_handle);
            CloseHandle(ov.hEvent);
            return false;
        }

        ok = GetOverlappedResult(m_handle, &ov, &written, FALSE);
    }

    CloseHandle(ov.hEvent);
    return ok && (written == static_cast<DWORD>(data.size()));
}

// ================================================================
//  Public write API
// ================================================================

bool SerialPort::write(const std::string &data)
{
    if (!isOpen())
        return false;
    {
        std::lock_guard lk(m_writeQueueMutex);
        m_writeQueue.push(WriteOp{data, INFINITE, nullptr});
    }
    m_writeQueueCV.notify_one();
    return true;
}

bool SerialPort::writeBlocking(const std::string &data, DWORD timeoutMs)
{
    if (!isOpen())
        return false;

    auto prom = std::make_shared<std::promise<bool>>();
    auto fut = prom->get_future();

    {
        std::lock_guard lk(m_writeQueueMutex);
        m_writeQueue.push(WriteOp{data, timeoutMs, prom});
    }
    m_writeQueueCV.notify_one();

    if (timeoutMs == INFINITE)
    {
        return fut.get();
    }

    // Add a small grace window beyond the actual write timeout
    const auto deadline = std::chrono::milliseconds(
        static_cast<long long>(timeoutMs) + 200);

    if (fut.wait_for(deadline) == std::future_status::ready)
        return fut.get();

    return false; // caller's deadline exceeded
}

// ================================================================
//  Public read API
// ================================================================

std::optional<std::string> SerialPort::readNonBlocking()
{
    std::lock_guard lk(m_rxMutex);
    if (m_rxBuffer.empty())
        return std::nullopt;
    return std::exchange(m_rxBuffer, {});
}

std::optional<std::string> SerialPort::readBlocking(DWORD timeoutMs)
{
    std::unique_lock lk(m_rxMutex);

    auto dataReady = [this]
    {
        return !m_rxBuffer.empty() || !m_running;
    };

    if (timeoutMs == INFINITE)
    {
        m_rxCV.wait(lk, dataReady);
    }
    else
    {
        if (!m_rxCV.wait_for(lk,
                             std::chrono::milliseconds(timeoutMs),
                             dataReady))
        {
            return std::nullopt; // timed out
        }
    }

    if (m_rxBuffer.empty())
        return std::nullopt;
    return std::exchange(m_rxBuffer, {});
}

// ================================================================
//  Callbacks
// ================================================================

void SerialPort::setReadCallback(ReadCallback cb)
{
    std::lock_guard lk(m_callbackMutex);
    m_readCallback = std::move(cb);
}

void SerialPort::setEventCallback(EventCallback cb)
{
    std::lock_guard lk(m_callbackMutex);
    m_eventCallback = std::move(cb);
}

// ================================================================
//  Control / modem lines
// ================================================================

bool SerialPort::setDTR(bool state)
{
    if (!isOpen())
        return false;
    return EscapeCommFunction(m_handle, state ? SETDTR : CLRDTR) != 0;
}

bool SerialPort::setRTS(bool state)
{
    if (!isOpen())
        return false;
    return EscapeCommFunction(m_handle, state ? SETRTS : CLRRTS) != 0;
}

namespace
{
    bool modemBit(HANDLE h, DWORD flag)
    {
        if (h == INVALID_HANDLE_VALUE)
            return false;
        DWORD status = 0;
        return GetCommModemStatus(h, &status) && ((status & flag) != 0);
    }
}

bool SerialPort::getCTS() const { return modemBit(m_handle, MS_CTS_ON); }
bool SerialPort::getDSR() const { return modemBit(m_handle, MS_DSR_ON); }
bool SerialPort::getRING() const { return modemBit(m_handle, MS_RING_ON); }
bool SerialPort::getCD() const { return modemBit(m_handle, MS_RLSD_ON); }
