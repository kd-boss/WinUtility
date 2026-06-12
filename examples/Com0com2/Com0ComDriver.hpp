#pragma once
// Com0ComDriver.hpp
//
// User-mode management of the com0com virtual null-modem driver.
//
// Enumeration:  SetupAPI — works without setupc.exe and is reliable
//               even when the tool is not on the PATH.
//
// Operations:   setupc.exe subprocess — avoids re-implementing the
//               driver's INF-based install/uninstall/change logic.
//
// Requires:  setupapi.lib   (auto-linked below)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinUtility/System.h>
#include <setupapi.h>
#include <devguid.h>
#ifdef _MSC_VER
#pragma comment(lib, "setupapi.lib")
#endif

#include <string>
#include <vector>
#include <optional>
#include <utility>

// ─── Data types ──────────────────────────────────────────────────────────────

/// One installed com0com null-modem pair.
struct Com0ComPair
{
    int pairIndex;          ///< 0-based pair number (0 = CNCA0/CNCB0, 1 = …)
    std::tstring cncNameA;  ///< e.g. L"CNCA0"
    std::tstring cncNameB;  ///< e.g. L"CNCB0"
    std::tstring portNameA; ///< e.g. L"COM10"  (or L"-" if unassigned)
    std::tstring portNameB; ///< e.g. L"COM11"  (or L"-" if unassigned)
    bool emuBrA = false;
    bool emuBrB = false;
};

/// Parameters for creating or editing a pair.
struct PairParams
{
    std::tstring portNameA; ///< Target port name, e.g. L"COM10"  (L"-" = auto)
    std::tstring portNameB; ///< Target port name, e.g. L"COM11"  (L"-" = auto)
    bool emuBrA = false;
    bool emuBrB = false;
};

// ─── Com0ComDriver ───────────────────────────────────────────────────────────

class Com0ComDriver
{
public:
    // ── Enumeration ──────────────────────────────────────────────────────────

    /// Returns all installed com0com pairs using SetupAPI.
    /// Does NOT require setupc.exe.  Returns an empty vector if the driver
    /// is not installed or no pairs have been created.
    static std::vector<Com0ComPair> EnumeratePairs();

    /// Returns the path to setupc.exe, searching common install locations
    /// and the directory of the current executable.
    /// Returns std::nullopt if setupc.exe cannot be found.
    static std::optional<std::tstring> FindSetupcPath();

    // ── Operations (all require an elevated process) ──────────────────────────

    /// Creates a new pair with the given port names and options.
    /// Equivalent to:  setupc.exe install PortName=A[,EmuBR=yes] PortName=B[,EmuBR=yes]
    static bool CreatePair(const PairParams &p, std::tstring &errorOut);

    /// Deletes the pair identified by @p pairIndex (e.g. 0 = CNCA0/CNCB0).
    /// Equivalent to:  setupc.exe uninstall CNCA{n}
    /// (uninstalling either side removes both)
    static bool DeletePair(int pairIndex, std::tstring &errorOut);

    /// Changes the port name(s) of an existing pair.
    /// Pass an empty string for a side to leave it unchanged.
    static bool ChangePorts(const Com0ComPair &pair,
                            const std::tstring &newPortA,
                            const std::tstring &newPortB,
                            std::tstring &errorOut);

    // ── Helpers ───────────────────────────────────────────────────────────────

    /// Returns true if com0com's driver is present in the device tree
    /// (i.e. at least one com0com device exists, even without any pairs).
    static bool IsDriverInstalled();

    /// Verifies that both sides of a pair have valid registry entries under
    /// HKLM\SYSTEM\CurrentControlSet\Enum\ROOT\PORTS\*\Device Parameters.
    /// Uses RegistryKey from System.h.  Returns true if both CNC names have
    /// a PortName value, indicating the driver successfully created the pair.
    static bool VerifyPairRegistry(const Com0ComPair &pair);

    /// Returns human-readable driver status for display in the UI.
    static std::tstring DriverStatusString();

private:
    // Runs setupc.exe with the given argument string.
    // Captures stdout+stderr into @p output.
    // Returns the process exit code, or -1 on launch failure.
    static int RunSetupc(const std::tstring &args, std::tstring &output);

    // Reads a named string value from a device's parameter registry key.
    static std::tstring ReadDeviceParamString(HDEVINFO hDev,
                                              SP_DEVINFO_DATA &devInfo,
                                              LPCTSTR valueName);

    // Parses "CNCA0" → pairIndex=0, side='A'
    // Parses "CNCB3" → pairIndex=3, side='B'
    // Returns false if the string doesn't match the CNC pattern.
    static bool ParseCncName(const std::tstring &name,
                             int &pairIndex, wchar_t &side);
};
