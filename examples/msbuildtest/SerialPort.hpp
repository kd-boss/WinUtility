#pragma once

// Requires: C++23 (std::to_underlying, using enum), Windows SDK
// Link:     setupapi.lib

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <utility> // std::to_underlying
#include <vector>

// ============================================================
//  Scoped enumerations
// ============================================================

enum class BaudRate : DWORD
{
    Baud110 = CBR_110,
    Baud300 = CBR_300,
    Baud600 = CBR_600,
    Baud1200 = CBR_1200,
    Baud2400 = CBR_2400,
    Baud4800 = CBR_4800,
    Baud9600 = CBR_9600,
    Baud14400 = CBR_14400,
    Baud19200 = CBR_19200,
    Baud38400 = CBR_38400,
    Baud57600 = CBR_57600,
    Baud115200 = CBR_115200,
    Baud128000 = CBR_128000,
    Baud256000 = CBR_256000,
};

enum class DataBits : BYTE
{
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
};

enum class StopBits : BYTE
{
    One = ONESTOPBIT,            // 0
    OnePointFive = ONE5STOPBITS, // 1
    Two = TWOSTOPBITS,           // 2
};

enum class Parity : BYTE
{
    None = NOPARITY,     // 0
    Odd = ODDPARITY,     // 1
    Even = EVENPARITY,   // 2
    Mark = MARKPARITY,   // 3
    Space = SPACEPARITY, // 4
};

enum class FlowControl : uint8_t
{
    None,
    Hardware, ///< RTS/CTS handshaking
    Software, ///< XON/XOFF
};

enum class PortEvent : uint8_t
{
    DataReceived,   ///< Byte(s) arrived (EV_RXCHAR)  — also fires read callback
    LineError,      ///< Line-status error             (EV_ERR)
    BreakDetected,  ///< Break signal                  (EV_BREAK)
    RingDetected,   ///< Ring indicator asserted        (EV_RING)
    CarrierChanged, ///< CD (RLSD) line changed         (EV_RLSD)
    CTSChanged,     ///< CTS line changed               (EV_CTS)
    DSRChanged,     ///< DSR line changed               (EV_DSR)
};

// ============================================================
//  Configuration aggregate  (all fields have sane defaults)
// ============================================================

struct SerialConfig
{
    BaudRate baudRate = BaudRate::Baud9600;
    DataBits dataBits = DataBits::Eight;
    StopBits stopBits = StopBits::One;
    Parity parity = Parity::None;
    FlowControl flowControl = FlowControl::None;
    DWORD writeTimeoutMs = 2'000; ///< Per-write timeout used by the write thread
    DWORD rxDriverBuf = 4'096;    ///< Win32 driver RX buffer hint (SetupComm)
    DWORD txDriverBuf = 4'096;    ///< Win32 driver TX buffer hint (SetupComm)
};

// ============================================================
//  SerialPort
// ============================================================
//
//  Thread model
//  ────────────
//  • Monitor thread  – uses WaitCommEvent (overlapped) to watch for
//    EV_RXCHAR and modem/line events.  On EV_RXCHAR it drains the
//    hardware FIFO into m_rxBuffer (std::string), wakes any callers
//    blocked in readBlocking(), and fires the optional read callback.
//
//  • Write thread    – serialises all pending writes so write() can
//    return immediately without the caller worrying about OVERLAPPED
//    lifetimes.  writeBlocking() enqueues the op with a promise and
//    waits on the associated future.
//
//  Usage at a glance
//  ─────────────────
//    SerialPort sp;
//    sp.setReadCallback ([](const std::string& s){ … });
//    sp.setEventCallback([](PortEvent e){ … });
//
//    SerialConfig cfg{ .baudRate = BaudRate::Baud115200 };
//    sp.open("COM3", cfg);
//
//    sp.write("ping\r\n");                       // non-blocking
//    sp.writeBlocking("pong\r\n", 1000);         // waits ≤ 1 s
//    auto rx = sp.readBlocking(2000);            // waits ≤ 2 s
// ============================================================

class SerialPort
{
public:
    using ReadCallback = std::function<void(const std::string &data)>;
    using EventCallback = std::function<void(PortEvent event)>;

    SerialPort() = default;
    ~SerialPort() { close(); }

    SerialPort(const SerialPort &) = delete;
    SerialPort &operator=(const SerialPort &) = delete;

    SerialPort(SerialPort &&) noexcept;
    SerialPort &operator=(SerialPort &&) noexcept;

    // --------------------------------------------------------
    //  Static helpers
    // --------------------------------------------------------

    /// Returns a sorted list of COM port names present on the system.
    static std::vector<std::string> enumeratePorts();

    // --------------------------------------------------------
    //  Lifecycle
    // --------------------------------------------------------

    /// Opens @p portName (e.g. "COM3") with the given config.
    /// Returns true on success.  Thread-safe to call from one thread.
    bool open(const std::string &portName, const SerialConfig &config = {});

    /// Flushes, signals both internal threads, joins them, then closes
    /// the Win32 handle.  Safe to call multiple times.
    void close();

    [[nodiscard]] bool isOpen() const noexcept;
    [[nodiscard]] const std::string &portName() const noexcept { return m_portName; }

    // --------------------------------------------------------
    //  Callbacks
    //  Can be set before or after open().  Both are invoked on
    //  the monitor thread — synchronise accordingly.
    // --------------------------------------------------------

    /// Fired for every chunk of bytes that arrives.
    /// The string contains exactly the bytes drained from the
    /// hardware FIFO in that delivery.
    void setReadCallback(ReadCallback cb);

    /// Fired for EV_ERR, EV_BREAK, EV_RING, EV_RLSD, EV_CTS, EV_DSR.
    /// DataReceived is NOT fired here; use setReadCallback instead.
    void setEventCallback(EventCallback cb);

    // --------------------------------------------------------
    //  Write
    // --------------------------------------------------------

    /// Non-blocking: enqueues @p data and returns immediately.
    /// Returns false only if the port is not open.
    bool write(const std::string &data);

    /// Blocking: enqueues @p data, waits up to @p timeoutMs for the
    /// write thread to complete the transmission, then returns.
    bool writeBlocking(const std::string &data, DWORD timeoutMs = INFINITE);

    // --------------------------------------------------------
    //  Read  (complements the callback; shares the same buffer)
    // --------------------------------------------------------

    /// Non-blocking: returns the internal buffer and clears it.
    /// Returns std::nullopt if the buffer is empty.
    [[nodiscard]] std::optional<std::string> readNonBlocking();

    /// Blocking: sleeps until at least one byte is available (or
    /// @p timeoutMs elapses), then returns the entire buffer.
    [[nodiscard]] std::optional<std::string> readBlocking(DWORD timeoutMs = INFINITE);

    // --------------------------------------------------------
    //  Modem / control lines
    // --------------------------------------------------------

    bool setDTR(bool state);
    bool setRTS(bool state);

    [[nodiscard]] bool getCTS() const;
    [[nodiscard]] bool getDSR() const;
    [[nodiscard]] bool getRING() const;
    [[nodiscard]] bool getCD() const;

private:
    // ---- Internal helpers ----
    bool applyConfig(const SerialConfig &cfg);
    bool doWrite(const std::string &data, DWORD timeoutMs);
    std::string drainHardwareBuffer();

    // ---- Monitor thread ----
    void startMonitorThread();
    void stopMonitorThread();
    void monitorThreadFunc();

    // ---- Write thread ----
    void startWriteThread();
    void stopWriteThread();
    void writeThreadFunc();

    // ---- Write-queue item ----
    struct WriteOp
    {
        std::string data;
        DWORD timeoutMs = INFINITE;
        std::shared_ptr<std::promise<bool>> pPromise; ///< null → fire-and-forget
    };

    // ---- Win32 handles ----
    HANDLE m_handle = INVALID_HANDLE_VALUE;
    HANDLE m_stopEvent = nullptr; ///< Manual-reset; signals both threads to exit

    // ---- Port metadata ----
    std::string m_portName;
    SerialConfig m_config;

    // ---- Callbacks ----
    ReadCallback m_readCallback;
    EventCallback m_eventCallback;
    mutable std::mutex m_callbackMutex; ///< Guards callback replacement at runtime

    // ---- Shared running flag ----
    std::atomic<bool> m_running{false};

    // ---- Monitor thread ----
    std::thread m_monitorThread;

    // ---- Write thread + queue ----
    std::thread m_writeThread;
    std::queue<WriteOp> m_writeQueue;
    std::mutex m_writeQueueMutex;
    std::condition_variable m_writeQueueCV;

    // ---- Internal RX buffer (filled by monitor thread) ----
    std::string m_rxBuffer;
    std::mutex m_rxMutex;
    std::condition_variable m_rxCV;
};
