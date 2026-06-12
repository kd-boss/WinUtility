#include "Com0ComDriver.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <sstream>
#include <shlwapi.h>
// ─── Internal helpers ────────────────────────────────────────────────────────

std::tstring Com0ComDriver::ReadDeviceParamString(HDEVINFO hDev,
                                                  SP_DEVINFO_DATA &devInfo,
                                                  LPCTSTR valueName)
{
    HKEY hKey = SetupDiOpenDevRegKey(hDev, &devInfo,
                                     DICS_FLAG_GLOBAL, 0,
                                     DIREG_DEV, KEY_READ);
    if (hKey == INVALID_HANDLE_VALUE)
        return TEXT("-");

    TCHAR buf[256]{};
    DWORD sz = sizeof(buf);
    DWORD type = 0;
    LONG rc = ::RegQueryValueEx(hKey, valueName, nullptr, &type,
                                reinterpret_cast<BYTE *>(buf), &sz);
    ::RegCloseKey(hKey);

    if (rc != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ))
        return TEXT("-");

    return buf;
}

bool Com0ComDriver::ParseCncName(const std::tstring &name,
                                 int &pairIndex, wchar_t &side)
{
    // Expected format: CNC{A|B}{digit(s)}
    // e.g. "CNCA0", "CNCB12"
    if (name.size() < 5)
        return false;
    if (name[0] != L'C' || name[1] != L'N' || name[2] != L'C')
        return false;

    wchar_t s = name[3];
    if (s != L'A' && s != L'B')
        return false;

    // digits follow
    for (std::size_t i = 4; i < name.size(); ++i)
        if (!iswdigit(name[i]))
            return false;

    side = s;
    pairIndex = std::stoi(name.substr(4));
    return true;
}

// ─── EnumeratePairs ──────────────────────────────────────────────────────────

std::vector<Com0ComPair> Com0ComDriver::EnumeratePairs()
{
    std::vector<Com0ComPair> result;

    HDEVINFO hDev = ::SetupDiGetClassDevs(
        &GUID_DEVCLASS_PORTS,
        nullptr, nullptr,
        DIGCF_PRESENT);

    if (hDev == INVALID_HANDLE_VALUE)
        return result;

    // key = pairIndex, value = partially-filled pair record
    std::map<int, Com0ComPair> pairMap;

    SP_DEVINFO_DATA devInfo{};
    devInfo.cbSize = sizeof(devInfo);

    for (DWORD idx = 0; ::SetupDiEnumDeviceInfo(hDev, idx, &devInfo); ++idx)
    {
        // Check hardware ID for "com0com"
        TCHAR hwId[512]{};
        if (!::SetupDiGetDeviceRegistryProperty(
                hDev, &devInfo, SPDRP_HARDWAREID,
                nullptr,
                reinterpret_cast<PBYTE>(hwId), sizeof(hwId), nullptr))
            continue;

        // Hardware IDs are a REG_MULTI_SZ — search each null-delimited string
        bool isCom0Com = false;
        for (LPCTSTR p = hwId; *p; p += _tcslen(p) + 1)
        {
            if (::StrStrI(p, TEXT("com0com")))
            {
                isCom0Com = true;
                break;
            }
        }
        if (!isCom0Com)
            continue;

        // Friendly name looks like "com0com - serial port emulator (CNCA0)"
        TCHAR friendly[256]{};
        ::SetupDiGetDeviceRegistryProperty(
            hDev, &devInfo, SPDRP_FRIENDLYNAME,
            nullptr,
            reinterpret_cast<PBYTE>(friendly), sizeof(friendly), nullptr);

        // Extract the CNC name from within the parentheses
        std::tstring friendlyStr(friendly);
        std::tstring cncName;
        auto lp = friendlyStr.rfind(L'(');
        auto rp = friendlyStr.rfind(L')');
        if (lp != std::tstring::npos && rp != std::tstring::npos && rp > lp)
            cncName = friendlyStr.substr(lp + 1, rp - lp - 1);

        if (cncName.empty())
            continue;

        int pairIdx = 0;
        wchar_t side = 0;
        if (!ParseCncName(cncName, pairIdx, side))
            continue;

        // Read the assigned COM port name from the device parameters
        std::tstring portName = ReadDeviceParamString(hDev, devInfo, TEXT("PortName"));

        // Read EmuBR flag (stored as "EmuBR" = "yes"/"no" in device params,
        // but since it's a driver param not a registry value, we can't read it
        // from the standard device param key — leave as false for display)
        // (setupc.exe list is the only reliable way to get these settings)

        auto &pair = pairMap[pairIdx];
        pair.pairIndex = pairIdx;

        if (side == L'A')
        {
            pair.cncNameA = cncName;
            pair.portNameA = portName;
        }
        else
        {
            pair.cncNameB = cncName;
            pair.portNameB = portName;
        }
    }

    ::SetupDiDestroyDeviceInfoList(hDev);

    // Flatten map into result vector, sorted by pairIndex
    result.reserve(pairMap.size());
    for (auto &[idx, pair] : pairMap)
    {
        // Fill in missing CNC names from index if not found in friendly name
        if (pair.cncNameA.empty())
            pair.cncNameA = std::format(TEXT("CNCA{}"), idx);
        if (pair.cncNameB.empty())
            pair.cncNameB = std::format(TEXT("CNCB{}"), idx);
        if (pair.portNameA.empty())
            pair.portNameA = TEXT("-");
        if (pair.portNameB.empty())
            pair.portNameB = TEXT("-");
        result.push_back(std::move(pair));
    }

    std::sort(result.begin(), result.end(),
              [](const Com0ComPair &a, const Com0ComPair &b)
              {
                  return a.pairIndex < b.pairIndex;
              });

    return result;
}

// ─── VerifyPairRegistry ──────────────────────────────────────────────────────
// Checks that both sides of a com0com pair have a "PortName" value in their
// Device Parameters registry key, confirming the driver created the pair and
// Windows has assigned COM port names to both sides.
//
// com0com stores device parameters under:
//   HKLM\SYSTEM\CurrentControlSet\Enum\ROOT\PORTS\<instanceId>\Device Parameters
// where the FriendlyName value contains the CNC name in parentheses.
// We walk the ROOT\PORTS subkeys looking for matching CNC names.

bool Com0ComDriver::VerifyPairRegistry(const Com0ComPair &pair)
{
    using namespace System;
    const std::tstring rootPath =
        TEXT("SYSTEM\\CurrentControlSet\\Enum\\ROOT\\PORTS");

    RegistryKey hklm{HKEY_LOCAL_MACHINE};
    RegistryKey rootKey = hklm.OpenKey(rootPath, KEY_READ | KEY_ENUMERATE_SUB_KEYS);
    if (!rootKey.IsValid())
        return false;

    bool foundA = false, foundB = false;

    // Enumerate all subkeys (instance IDs like "0000", "0001", ...)
    DWORD index = 0;
    TCHAR subkeyName[256]{};
    DWORD subkeyLen = static_cast<DWORD>(_countof(subkeyName));

    while (::RegEnumKeyEx(rootKey.Get(), index++, subkeyName, &subkeyLen,
                          nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
    {
        subkeyLen = static_cast<DWORD>(_countof(subkeyName));

        RegistryKey instKey = rootKey.OpenKey(subkeyName, KEY_READ);
        if (!instKey.IsValid())
            continue;

        // Read FriendlyName to identify which CNC device this is
        std::tstring friendly = instKey.GetString(TEXT(""), TEXT("FriendlyName"));
        if (friendly.empty())
            continue;

        // Extract CNC name from parentheses e.g. "com0com - ... (CNCA0)"
        auto lp = friendly.rfind(L'(');
        auto rp = friendly.rfind(L')');
        if (lp == std::tstring::npos || rp == std::tstring::npos || rp <= lp)
            continue;
        std::tstring cncName = friendly.substr(lp + 1, rp - lp - 1);

        if (cncName != pair.cncNameA && cncName != pair.cncNameB)
            continue;

        // Found a matching CNC device — verify Device Parameters\PortName exists
        RegistryKey devParams = instKey.OpenKey(TEXT("Device Parameters"), KEY_READ);
        if (!devParams.IsValid())
            continue;

        std::tstring portName = devParams.GetString(TEXT(""), TEXT("PortName"));
        if (portName.empty() || portName == TEXT("-"))
            continue;

        if (cncName == pair.cncNameA)
            foundA = true;
        if (cncName == pair.cncNameB)
            foundB = true;
        if (foundA && foundB)
            break;
    }

    return foundA && foundB;
}

// ─── IsDriverInstalled / DriverStatusString ───────────────────────────────────

bool Com0ComDriver::IsDriverInstalled()
{
    // Check for the presence of any device with hardware ID containing "com0com".
    // This covers the case where the driver is installed but no pairs exist yet.
    HDEVINFO hDev = ::SetupDiGetClassDevs(
        nullptr, TEXT("com0com"), nullptr,
        DIGCF_ALLCLASSES | DIGCF_PRESENT);

    if (hDev == INVALID_HANDLE_VALUE)
    {
        // Also try enumerating ports
        auto pairs = EnumeratePairs();
        return !pairs.empty();
    }

    SP_DEVINFO_DATA devInfo{};
    devInfo.cbSize = sizeof(devInfo);
    bool found = ::SetupDiEnumDeviceInfo(hDev, 0, &devInfo) != FALSE;
    ::SetupDiDestroyDeviceInfoList(hDev);

    if (!found)
    {
        // Fall back to checking via serial port class
        auto pairs = EnumeratePairs();
        found = !pairs.empty();
    }
    return found;
}

std::tstring Com0ComDriver::DriverStatusString()
{
    auto setupc = FindSetupcPath();
    std::tstring status;

    if (IsDriverInstalled())
        status = TEXT("com0com driver: installed  |  ");
    else
        status = TEXT("com0com driver: NOT installed  |  ");

    if (setupc)
        status += TEXT("setupc.exe: found");
    else
        status += TEXT("setupc.exe: NOT found  (add its folder to PATH or copy to app directory)");

    return status;
}

// ─── FindSetupcPath ──────────────────────────────────────────────────────────

std::optional<std::tstring> Com0ComDriver::FindSetupcPath()
{
    // 1. Same directory as the running executable
    TCHAR exePath[MAX_PATH]{};
    ::GetModuleFileName(nullptr, exePath, MAX_PATH);
    TCHAR *lastSlash = ::StrRChr(exePath, nullptr, L'\\');
    if (lastSlash)
    {
        *(lastSlash + 1) = L'\0';
        std::tstring candidate = std::tstring(exePath) + TEXT("setupc.exe");
        if (::GetFileAttributes(candidate.c_str()) != INVALID_FILE_ATTRIBUTES)
            return candidate;
    }

    // 2. Common installation directories
    const TCHAR *candidates[] = {
        TEXT("C:\\Program Files\\com0com\\setupc.exe"),
        TEXT("C:\\Program Files (x86)\\com0com\\setupc.exe"),
        TEXT("C:\\com0com\\setupc.exe"),
    };
    for (auto c : candidates)
    {
        if (::GetFileAttributes(c) != INVALID_FILE_ATTRIBUTES)
            return std::tstring(c);
    }

    // 3. Registry: check the uninstall key for the install location
    const TCHAR *uninstKey =
        TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\com0com");
    HKEY hKey = nullptr;
    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, uninstKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        TCHAR installDir[MAX_PATH]{};
        DWORD sz = sizeof(installDir);
        DWORD type = 0;
        if (::RegQueryValueEx(hKey, TEXT("InstallLocation"), nullptr, &type,
                              reinterpret_cast<BYTE *>(installDir), &sz) == ERROR_SUCCESS)
        {
            std::tstring candidate = std::tstring(installDir) + TEXT("\\setupc.exe");
            ::RegCloseKey(hKey);
            if (::GetFileAttributes(candidate.c_str()) != INVALID_FILE_ATTRIBUTES)
                return candidate;
        }
        else
        {
            ::RegCloseKey(hKey);
        }
    }

    return std::nullopt;
}

// ─── RunSetupc ───────────────────────────────────────────────────────────────

int Com0ComDriver::RunSetupc(const std::tstring &args, std::tstring &output)
{
    auto setupcPath = FindSetupcPath();
    if (!setupcPath)
    {
        output = TEXT("setupc.exe not found.  "
                      "Install com0com or place setupc.exe in the application directory.");
        return -1;
    }

    std::tstring cmdLine = TEXT("\"") + *setupcPath + TEXT("\" ") + args;

    // Create an anonymous pipe to capture stdout and stderr
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;

    HANDLE hReadPipe = nullptr;
    HANDLE hWritePipe = nullptr;
    if (!::CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
    {
        output = TEXT("Failed to create pipe for setupc.exe.");
        return -1;
    }
    // Make the read end non-inheritable
    ::SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFO si{};
    si.cb = sizeof(si);
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi{};
    std::vector<TCHAR> cmdBuf(cmdLine.begin(), cmdLine.end());
    cmdBuf.push_back(L'\0');

    BOOL ok = ::CreateProcess(
        nullptr, cmdBuf.data(),
        nullptr, nullptr,
        /*bInheritHandles=*/TRUE,
        CREATE_NO_WINDOW,
        nullptr, nullptr,
        &si, &pi);

    ::CloseHandle(hWritePipe); // must close before reading or ReadFile blocks

    if (!ok)
    {
        ::CloseHandle(hReadPipe);
        output = TEXT("Failed to launch setupc.exe.");
        return -1;
    }

    // Drain the pipe
    std::string rawOutput;
    char buf[4096];
    DWORD bytesRead = 0;
    while (::ReadFile(hReadPipe, buf, sizeof(buf) - 1, &bytesRead, nullptr) && bytesRead > 0)
    {
        buf[bytesRead] = '\0';
        rawOutput += buf;
    }
    ::CloseHandle(hReadPipe);

    // Wait for setupc.exe to finish
    ::WaitForSingleObject(pi.hProcess, 10'000); // 10-second safety timeout

    DWORD exitCode = static_cast<DWORD>(-1);
    ::GetExitCodeProcess(pi.hProcess, &exitCode);

    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);

    // Convert stdout (UTF-8 or ANSI) to tstring
    if (!rawOutput.empty())
    {
        int wLen = ::MultiByteToWideChar(CP_ACP, 0,
                                         rawOutput.c_str(),
                                         static_cast<int>(rawOutput.size()),
                                         nullptr, 0);
        if (wLen > 0)
        {
            std::wstring wOut(wLen, L'\0');
            ::MultiByteToWideChar(CP_ACP, 0,
                                  rawOutput.c_str(),
                                  static_cast<int>(rawOutput.size()),
                                  wOut.data(), wLen);
            output = wOut;
        }
    }

    return static_cast<int>(exitCode);
}

// ─── CreatePair ──────────────────────────────────────────────────────────────

bool Com0ComDriver::CreatePair(const PairParams &p, std::tstring &errorOut)
{
    // setupc.exe install  PortName=COM10[,EmuBR=yes]  PortName=COM11[,EmuBR=yes]
    //
    // Using "-" as the bus parameter means "auto-assign device number".
    // If portNameA/B is empty or "-" the driver picks the name automatically.

    auto makePortArg = [](const std::tstring &name, bool emuBr) -> std::tstring
    {
        std::tstring arg = TEXT("PortName=") + (name.empty() ? TEXT("-") : name);
        if (emuBr)
            arg += TEXT(",EmuBR=yes");
        return arg;
    };

    std::tstring args = TEXT("install - ") +
                        makePortArg(p.portNameA, p.emuBrA) + TEXT(" ") +
                        makePortArg(p.portNameB, p.emuBrB);

    std::tstring output;
    int rc = RunSetupc(args, output);
    if (rc != 0)
    {
        errorOut = std::format(TEXT("setupc.exe install failed (exit {}): {}"), rc, output);
        return false;
    }
    return true;
}

// ─── DeletePair ──────────────────────────────────────────────────────────────

bool Com0ComDriver::DeletePair(int pairIndex, std::tstring &errorOut)
{
    // setupc.exe uninstall CNCA{n}
    // Removing either side removes both.
    std::tstring args = std::format(TEXT("uninstall CNCA{}"), pairIndex);

    std::tstring output;
    int rc = RunSetupc(args, output);
    if (rc != 0)
    {
        errorOut = std::format(TEXT("setupc.exe uninstall failed (exit {}): {}"), rc, output);
        return false;
    }
    return true;
}

// ─── ChangePorts ─────────────────────────────────────────────────────────────

bool Com0ComDriver::ChangePorts(const Com0ComPair &pair,
                                const std::tstring &newPortA,
                                const std::tstring &newPortB,
                                std::tstring &errorOut)
{
    // setupc.exe change CNCA{n} PortName=COM10
    // setupc.exe change CNCB{n} PortName=COM11
    std::tstring output;

    if (!newPortA.empty() && newPortA != pair.portNameA)
    {
        std::tstring args = std::format(TEXT("change {} PortName={}"),
                                        pair.cncNameA, newPortA);
        int rc = RunSetupc(args, output);
        if (rc != 0)
        {
            errorOut = std::format(TEXT("Failed to change {} (exit {}): {}"),
                                   pair.cncNameA, rc, output);
            return false;
        }
    }

    if (!newPortB.empty() && newPortB != pair.portNameB)
    {
        std::tstring args = std::format(TEXT("change {} PortName={}"),
                                        pair.cncNameB, newPortB);
        int rc = RunSetupc(args, output);
        if (rc != 0)
        {
            errorOut = std::format(TEXT("Failed to change {} (exit {}): {}"),
                                   pair.cncNameB, rc, output);
            return false;
        }
    }

    return true;
}