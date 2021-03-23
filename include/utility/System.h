
#ifndef __SYSTEM__
#define __SYSTEM__
#include <Windows.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// usefull system typedef's
typedef BYTE byte;

// usefull debugging macro's
#if __cplusplus < 201103L
#error This file requires compiler and library support for the \
ISO C++ 2011 standard. This support is currently experimental, and must be \
enabled with the -std=c++11 or -std=gnu++11 compiler options.
#endif

#ifndef ASSERT
#define ASSERT(cond) _ASSERTE(cond)
#if WINAPI_FAMILY_PHONE_APP == WINAPI_FAMILY
#ifdef _DEBUG
#undef ASSERT
#define ASSERT(expression)                                                                                             \
    {                                                                                                                  \
        if (!(expression))                                                                                             \
            throw Platform::Exception::CreateException(E_FAIL);                                                        \
    }
#endif
#endif
#endif

#ifndef VERIFY
#ifdef _DEBUG
#define VERIFY(expression) ASSERT(expression)
#else
#define VERIFY(expression) (expression)
#endif
#endif
namespace System
{
#ifndef _TRACE
#ifdef _DEBUG
#define _TRACE
#include <stdio.h>
inline void TRACE(WCHAR const *const format, ...)
{
    va_list args;
    va_start(args, format);
    WCHAR output[512];
    vswprintf_s(output, format, args);
    OutputDebugStringW(output);
    va_end(args);
}
inline void TRACE(CHAR const *const format, ...)
{
    va_list args;
    va_start(args, format);
    CHAR output[512];
    vsprintf_s(output, format, args);
    OutputDebugStringA(output);
    va_end(args);
}
#else
#define _TRACE
#define TRACE
#endif
#endif
} // namespace System
#ifndef NTVERIFY
#ifdef _DEBUG
#define NTVERIFY(expression) ASSERT((((NTSTATUS)(expression)) >= 0))
#else
#define NTVERIFY(expression) (expression)
#endif
#endif
#endif

namespace System
{

class Utf8ConversionException : public std::runtime_error
{
    uint32_t _errorCode;

  public:
    Utf8ConversionException(const char *message, uint32_t errorCode)
        : std::runtime_error(message), _errorCode(errorCode)
    {
    }

    uint32_t ErrorCode() const
    {
        return _errorCode;
    }
};

std::wstring to_wstring(std::string a)
{
    std::wstring ret;
    if (a.empty())
    {
        return ret;
    }
    const DWORD kFlags = MB_ERR_INVALID_CHARS;
    if (a.length() > static_cast<size_t>(std::numeric_limits<int>::max()))
    {
        throw std::overflow_error("Input string too long: size_t length doesn't fit into int.");
    }
    int inputLen = static_cast<int>(a.length());
    int outLen = ::MultiByteToWideChar(CP_UTF8, kFlags, a.data(), inputLen, nullptr, 0);
    if (outLen == 0)
    {
        const DWORD error = ::GetLastError();
        throw Utf8ConversionException(
            "Cannot get result string length when converting from UTF-8 to UTF-16 (MultiByteToWideChar failed).",
            error);
    }
    ret.resize(outLen);
    outLen = ::MultiByteToWideChar(CP_UTF8, kFlags, a.data(), inputLen, &ret[0], ret.length());
    if (outLen == 0)
    {
        // Conversion error: capture error code and throw
        const DWORD error = ::GetLastError();
        throw Utf8ConversionException("Cannot convert from UTF-8 to UTF-16 "
                                      "(MultiByteToWideChar failed).",
                                      error);
    }
    return ret;
}

namespace Information
{
#ifdef UNICODE
typedef std::true_type isUnicode;
#ifndef __GNUG__
#ifdef DEBUG
#pragma comment(lib, "comsuppwd.lib")
#else
#pragma comment(lib, "comsuppw.lib")
#endif
#endif
#else
typedef std::false_type isUnicode;
#ifndef __GNUG__
#ifdef DEBUG
#pragma comment(lib, "comsuppd.lib")
#else
#pragma comment(lib, "comsupp.lib")
#endif
#endif
#endif

#if defined(_M_X64) || defined(__x86_64__)
typedef std::true_type is64Bit;
#endif
#if defined(_M_IX86) || defined(_X86_)
typedef std::false_type is64Bit;
#endif
#if defined(DEBUG) || defined(_DEBUG)
typedef std::true_type isDebug;
#else
typedef std::false_type isDebug;
#endif
#ifdef _CPPUNWIND
typedef std::true_type hasExceptionHandeling;
#else
typedef std::false_type hasExceptionHandeling;
#endif
#if defined(_CPPRTTI) || defined(__GXX_RTTI)
typedef std::true_type hasRuntimeTypeInformation;
#else
typedef std::false_type hasRuntimeTypeInformation;
#endif
#ifdef _MSC_VER
#if (_HAS_ITERATOR_DEBUGGING == 1)
typedef std::true_type hasIteratorChecking;
#else
typedef std::false_type hasIteratorChecking;
#endif
#endif

#ifdef _MSC_VER
#ifdef _CPPLIB_VER
typedef std::true_type hasStdLib;
typedef std::integral_constant<int, _CPPLIB_VER> StdVer;
#else
typedef std::false_type hasStdLib;
typedef std::integral_constant<int, 0> StdVer;
#endif
#else
#ifdef __GLIBC__
typedef integral_constant<int, __GLIBC__> StdVer;
#endif
#ifdef __GNU_LIBRARY__
typedef integral_constant<int, __GNU_LIBRARY__> StdVer;
#endif
#endif
#ifdef _MSC_VER
#ifdef _DLL
typedef std::true_type hasMultiThreadedStd;
#else
typedef std::false_type hasMutiThreadedStd;
#endif

typedef std::integral_constant<int, _INTEGRAL_MAX_BITS> IntMaxBits;

#if (_M_IX86_FP == 0)
typedef std::false_type hasSSE;
typedef std::false_type hasSSE2;
#endif
#if (_M_IX86_FP == 1)
typedef std::true_type hasSSE;
typedef std::false_type hasSSE2;
#endif
#if (_M_IX86_FP == 2)
typedef std::false_type hasSSE;
typedef std::true_type hasSSE2;
#endif

#else
#ifdef __SSE__
typedef std::true_type hasSSE;
#else
typedef std::false_type hasSEE;
#endif
#ifdef __SSE2__
typedef std::true_type hasSEE2;
#else
typedef std::false_type hasSEE2;
#endif

#ifdef __SSE3__
typedef std::true_type hasSSE3;
#else
typedef std::false_type hasSSE3;
#endif

#endif

#ifdef NTDDI_VERSION
typedef std::integral_constant<long, NTDDI_VERSION> WinVer;
#else
typedef std::integral_constant<long, NTDDI_VERSION_FROM_WIN32_WINNT(WINVER)> WinVer;
#endif

typedef std::integral_constant<long, NTDDI_WIN8> Win8;
typedef std::integral_constant<long, NTDDI_WIN7> Win7;
typedef std::integral_constant<long, NTDDI_VISTA> WinVista;
typedef std::integral_constant<long, NTDDI_WINXP> WinXp;
typedef std::integral_constant<long, NTDDI_WINXPSP1> WinXpSp1;
typedef std::integral_constant<long, NTDDI_WINXPSP2> WinXpSp2;
typedef std::integral_constant<long, NTDDI_WINXPSP3> WinXpSp3;
typedef std::integral_constant<long, NTDDI_LONGHORN> WinLongHorn;

typedef std::integral_constant<long, _WIN32_WINNT_WIN2K> Win2k;
typedef std::integral_constant<long, NTDDI_WS03> WinServer03;
typedef std::integral_constant<long, NTDDI_WS03SP1> WinServer03Sp1;
typedef std::integral_constant<long, NTDDI_WS03SP2> WinServer03Sp2;
typedef std::integral_constant<long, NTDDI_WS03SP3> WinServer03Sp3;
typedef std::integral_constant<long, NTDDI_WS03SP4> WinServer03Sp4;
typedef std::integral_constant<long, NTDDI_WS08> WinServer08;
}; // namespace Information

typedef std::vector<byte> ByteVector;
#ifndef __SYSTEM_STRINGTOBYTEVECTOR
#define __SYSTEM_STRINGTOBYTEVECTOR
inline std::vector<byte> StringToByteVector(std::string s)
{
    return std::vector<byte>(std::begin(s), std::end(s));
}
inline std::string ByteVectorToString(ByteVector v)
{
    return std::string(std::begin(v), std::end(v));
}
#endif
namespace Debug
{
#ifndef __SYSTEM_DEBUG_PRINT_ERROR__
#define __SYSTEM_DEBUG_PRINT_ERROR__
inline void PrintError(std::string error)
{
    TRACE(error.c_str(), "%s\r\n");
}
inline void PrintError(std::wstring error)
{
    TRACE(error.c_str(), "%s\r\n");
}
#endif
}; // namespace Debug

}; // namespace System
