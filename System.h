
#ifndef __SYSTEM__
#define __SYSTEM__
#include <Windows.h>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <utility>



//usefull system typedef's
typedef BYTE byte;

//usefull debugging macro's

#ifndef ASSERT
#define ASSERT(cond) _ASSERTE(cond)
#if WINAPI_FAMILY_PHONE_APP == WINAPI_FAMILY
#ifdef _DEBUG
#undef ASSERT
#define ASSERT(expression) { if (!(expression)) throw Platform::Exception::CreateException(E_FAIL); }
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
namespace System {
#ifndef _TRACE
#ifdef _DEBUG
#define _TRACE
#include <stdio.h>
	inline void TRACE(WCHAR const * const format, ...)
	{
		va_list args;
		va_start(args, format);
		WCHAR output[512];
		vswprintf_s(output, format, args);
		OutputDebugStringW(output);
		va_end(args);
	}
	inline void TRACE(CHAR const * const format, ...)
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
}
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
	namespace Information{
#ifdef UNICODE
		typedef std::true_type isUnicode;
#ifndef __GNUG__
#ifdef DEBUG
#pragma comment(lib,"comsuppwd.lib")
#else
#pragma comment(lib,"comsuppw.lib")
#endif
#endif
#else
		typedef std::false_type isUnicode;
#ifndef __GNUG__
#ifdef DEBUG
#pragma comment(lib,"comsuppd.lib")
#else
#pragma comment(lib,"comsupp.lib")
#endif
#endif
#endif

#ifdef _M_X64
		typedef std::true_type is64Bit;
#endif
#ifdef _M_IX86
		typedef std::false_type is64Bit;
#endif
#ifdef DEBUG
		typedef std::true_type isDebug;
#else
		typedef std::false_type isDebug;
#endif
#ifdef _CPPUNWIND
		typedef std::true_type hasExceptionHandeling;
#else
		typedef std::false_type hasExceptionHandeling;
#endif
#ifdef _CPPRTTI
		typedef std::true_type hasRuntimeTypeInformation;
#else
		typedef std::false_type hasRuntimeTypeInformation;
#endif
#if(_HAS_ITERATOR_DEBUGGING == 1)
		typedef std::true_type hasIteratorChecking;
#else
		typedef std::false_type hasIteratorChecking;
#endif
#ifdef _CPPLIB_VER
		typedef std::true_type hasStdLib;
		typedef std::integral_constant<int, _CPPLIB_VER> StdVer;
#else
		typedef std::false_type hasStdLib;
		typedef std::integral_constant<int, 0> StdVer;
#endif
#ifdef _DLL
		typedef std::true_type hasMultiThreadedStd;
#else
		typedef std::false_type hasMutiThreadedStd;
#endif
		typedef std::integral_constant<int, _INTEGRAL_MAX_BITS> IntMaxBits;
#if(_M_IX86_FP == 0)
		typedef std::false_type hasSSE;
		typedef std::false_type hasSSE2;
#endif
#if(_M_IX86_FP == 1)
		typedef std::true_type hasSSE;
		typedef std::false_type hasSSE2;
#endif
#if(_M_IX86_FP == 2)
		typedef std::false_type hasSSE;
		typedef std::true_type hasSSE2;
#endif
#ifdef NTDDI_VERSION
		typedef std::integral_constant<long, NTDDI_VERSION>		WinVer;
#else
		typedef std::integral_constant<long, NTDDI_VERSION_FROM_WIN32_WINNT(WINVER)> WinVer;
#endif

		typedef std::integral_constant<long, NTDDI_WIN8>	Win8;
		typedef std::integral_constant<long, NTDDI_WIN7>	Win7;
		typedef std::integral_constant<long, NTDDI_VISTA>	WinVista;
		typedef std::integral_constant<long, NTDDI_WINXP>	WinXp;
		typedef std::integral_constant<long, NTDDI_WINXPSP1> WinXpSp1;
		typedef std::integral_constant<long, NTDDI_WINXPSP2> WinXpSp2;
		typedef std::integral_constant<long, NTDDI_WINXPSP3> WinXpSp3;
		typedef std::integral_constant<long, NTDDI_LONGHORN> WinLongHorn;

		typedef std::integral_constant<long, _WIN32_WINNT_WIN2K>	Win2k;
		typedef std::integral_constant<long, NTDDI_WS03>	WinServer03;
		typedef std::integral_constant<long, NTDDI_WS03SP1> WinServer03Sp1;
		typedef std::integral_constant<long, NTDDI_WS03SP2> WinServer03Sp2;
		typedef std::integral_constant<long, NTDDI_WS03SP3> WinServer03Sp3;
		typedef std::integral_constant<long, NTDDI_WS03SP4> WinServer03Sp4;
		typedef std::integral_constant<long, NTDDI_WS08>	WinServer08;
	};

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
			TRACE(error.c_str(),"%s\r\n");
		}
		inline void PrintError(std::wstring error)
		{
			TRACE(error.c_str(), "%s\r\n");
		}
#endif
	};


};

