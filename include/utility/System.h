
#ifndef __SYSTEM__
#define __SYSTEM__
#include <Windows.h>
#include <winnt.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <iostream>
#include <ktmw32.h>
#include <cstring>
#include <tuple>
#include <numeric>

// usefull debugging macro's
#if __cplusplus < 201103L
#error This file requires compiler and library support for the \
ISO C++ 2011 standard.
#endif

#ifndef ASSERT
#define ASSERT(cond) _ASSERTE(cond)
#if WINAPI_FAMILY_PHONE_APP == WINAPI_FAMILY
#ifdef _DEBUG
#undef ASSERT
#define ASSERT(expression)                                                                                             \
    {                                                                                                                  \
        if (!(expression))                                                                                             \
            
    }
#endif
#endif


#ifndef VERIFY
#ifdef _DEBUG
#define VERIFY(expression) ASSERT(expression)
#else
#define VERIFY(expression) (expression)
#endif
#endif

#ifndef _tcslen(x)
#if UNICODE
#define _tcslen(x) wcslen(x)
#else
#define _tcslen(x) strlen(x)
#endif
#endif

#ifndef NTVERIFY
#ifdef _DEBUG
#define NTVERIFY(expression) ASSERT((((NTSTATUS)(expression)) >= 0))
#else
#define NTVERIFY(expression) (expression)
#endif
#endif

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

namespace System
{
std::wstring to_wstring(std::string);
std::string to_string(std::wstring);	


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
#ifndef __GNUG__
#ifdef DEBUG
#pragma comment(lib, "comsuppd.lib")
#else
#pragma comment(lib, "comsupp.lib")
#endif
#endif
#else
typedef std::false_type isUnicode;
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
struct StdVer : std::integral_constant<int, _CPPLIB_VER> {};
#else
typedef std::false_type hasStdLib;
struct StdVer : std::integral_constant<int, 0> StdVer;
#endif
#else
#ifdef __GLIBC__
struct StdVer : integral_constant<int, __GLIBC__> StdVer;
#endif
#ifdef __GNU_LIBRARY__
struct StdVer : integral_constant<int, __GNU_LIBRARY__> StdVer;
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
struct WinVer : std::integral_constant<long, NTDDI_VERSION> {};
#else
struct WinVer : std::integral_constant<long, NTDDI_VERSION_FROM_WIN32_WINNT(WINVER)> {};
#endif

struct 	WinXp:  std::integral_constant<long, NTDDI_WINXP> 			    {};
struct 	WinXpSp1:  std::integral_constant<long, NTDDI_WINXPSP1> 		{};
struct 	WinXpSp2:  std::integral_constant<long, NTDDI_WINXPSP2> 		{};
struct 	WinXpSp3:  std::integral_constant<long, NTDDI_WINXPSP3> 		{};
struct 	WinLongHorn:  std::integral_constant<long, NTDDI_LONGHORN> 		{};	
struct 	WinVista:  std::integral_constant<long, NTDDI_WIN6> 			{};
struct  WinVistaSp1 : std::integral_constant<long, NTDDI_WIN6SP1>      	{};
struct  WinVistaSp2 : std::integral_constant<long, NTDDI_WIN6SP2>		{};
struct  WinVistaSp3 : std::integral_constant<long, NTDDI_WIN6SP3>		{};
struct  WinVistaSp4 : std::integral_constant<long, NTDDI_WIN6SP4>		{};
struct 	Win7:  std::integral_constant<long, NTDDI_WIN7> 			    {};
struct 	Win8:  std::integral_constant<long, NTDDI_WIN8> 			    {};
struct  Win8_1 : std::integral_constant<long, NTDDI_WINBLUE> 			{};
struct  Win10_1511 : std::integral_constant<long, NTDDI_WIN10_TH2> 		{};
struct  Win10_1607 : std::integral_constant<long, NTDDI_WIN10_RS1> 		{};
struct  Win10_1703 : std::integral_constant<long, NTDDI_WIN10_RS2> 		{};
struct  Win10_1709 : std::integral_constant<long, NTDDI_WIN10_RS3>		{};
struct  Win10_1803 : std::integral_constant<long, NTDDI_WIN10_RS4>		{};
struct  Win10_1809 : std::integral_constant<long, NTDDI_WIN10_RS5>		{};
struct  Win10_1903 : std::integral_constant<long, NTDDI_WIN10_19H1>     {};
struct  Win10 : std::integral_constant<long, NTDDI_WIN10> {};
struct  Win10_TH2 : std::integral_constant<long, NTDDI_WIN10_TH2> {};
struct  Win10_RS1 : std::integral_constant<long, NTDDI_WIN10_RS1> {};
struct  Win10_RS2 : std::integral_constant<long, NTDDI_WIN10_RS2> {};
struct  Win10_RS3 : std::integral_constant<long, NTDDI_WIN10_RS3> {};
struct  Win10_RS4 : std::integral_constant<long, NTDDI_WIN10_RS4> {};
struct  Win10_RS5 : std::integral_constant<long, NTDDI_WIN10_RS5> {};
struct  Win10_19H1 : std::integral_constant<long, NTDDI_WIN10_19H1> {};

struct 	Win2k:  std::integral_constant<long, _WIN32_WINNT_WIN2K> 	    {};
struct 	WinServer03:  std::integral_constant<long, NTDDI_WS03> 			{};
struct 	WinServer03Sp1:  std::integral_constant<long, NTDDI_WS03SP1> 	{};	
struct 	WinServer03Sp2:  std::integral_constant<long, NTDDI_WS03SP2> 	{};	
struct 	WinServer03Sp3:  std::integral_constant<long, NTDDI_WS03SP3> 	{};	
struct 	WinServer03Sp4:  std::integral_constant<long, NTDDI_WS03SP4> 	{};	
struct 	WinServer08:  std::integral_constant<long, NTDDI_WS08> 	        {};

} // namespace Information


}

namespace std
{
#ifdef __GNUC__
#if __cplusplus <= 201103L

template <class T> struct _Unique_if
{
    typedef unique_ptr<T> _Single_object;
};

template <class T> struct _Unique_if<T[]>
{
    typedef unique_ptr<T[]> _Unknown_bound;
};

template <class T, size_t N> struct _Unique_if<T[N]>
{
    typedef void _Known_bound;
};

template <class T, class... Args> typename _Unique_if<T>::_Single_object make_unique(Args &&...args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T> typename _Unique_if<T>::_Unknown_bound make_unique(size_t n)
{
    typedef typename remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]());
}

template <class T, class... Args> typename _Unique_if<T>::_Known_bound make_unique(Args &&...) = delete;

#endif
#endif


#ifdef UNICODE
	typedef ::std::wstring tstring;
	template<typename T>
	tstring to_tstring(T t)
	{
		return std::to_wstring(std::forward<T>(t));
	}
	
	template<>
	tstring to_tstring(const char* s)
	{
		return System::to_wstring(s);
	}
	
	template<>
	tstring to_tstring(const wchar_t* s)
	{
		return tstring(s);
	}
	
	typedef std::wstringstream tstringstream;
#else
	typedef ::std::string tstring;
	template<typename T>
	tstring to_tstring(T t)
	{
		return std::to_string(std::forward<T>(t));
	}
	template<>
	tstring to_tstring(const wchar_t* s)
	{
		return System::to_string(s);
	}
	
	template<>
	tstring to_tstring(const char* s)
	{
		return tstring(s);
	}
	
	typedef std::stringstream tstringstream;
#endif

#ifdef __GNUC__
#if __cplusplus <= 201103L

template <class T> struct _Unique_if
{
    typedef unique_ptr<T> _Single_object;
};

template <class T> struct _Unique_if<T[]>
{
    typedef unique_ptr<T[]> _Unknown_bound;
};

template <class T, size_t N> struct _Unique_if<T[N]>
{
    typedef void _Known_bound;
};

template <class T, class... Args> typename _Unique_if<T>::_Single_object make_unique(Args &&...args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T> typename _Unique_if<T>::_Unknown_bound make_unique(size_t n)
{
    typedef typename remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]());
}

template <class T, class... Args> typename _Unique_if<T>::_Known_bound make_unique(Args &&...) = delete;
#endif
#endif
} // namespace std


namespace System
{



class SystemException : std::exception
{
	std::tstring localbuff;
    PTSTR buffer;
	const char * m_file;
	int linenum;
	void Swap(SystemException& rhs) noexcept
	{
		using std::swap;
		swap(buffer, rhs.buffer);
		swap(m_file, rhs.m_file);
		int linenum = rhs.linenum;
		swap(localbuff, rhs.localbuff);
	}
 public:
    DWORD m_val;
	HRESULT m_hval;	
	
    SystemException(DWORD val, const char* file, int line) noexcept : m_val(val), m_hval(0), buffer(nullptr), m_file(file), linenum(line) 
    {
    }

	SystemException(const SystemException& rhs) = delete;

	SystemException(SystemException&& rhs) noexcept = default;

	SystemException& operator=(SystemException& rhs) noexcept
	{
		rhs.Swap(*this);
		return *this;
	}

    PCTSTR what() noexcept
    {
		std::tstringstream out;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,		
                      nullptr, m_val, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buffer, 0, nullptr);
		
#ifdef UNIOCDE
			out <<	System::to_wstring(m_file);
#else
			out << std::to_tstring(m_file); 
#endif
		out << TEXT(":");
		out << std::to_tstring(linenum);
        out	<< std::endl;
		out << buffer;
		localbuff = out.str();
		return localbuff.c_str();
    }
	
    ~SystemException() noexcept
    {
        if (buffer)
            ::LocalFree(buffer);
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
        throw SystemException{error, __FILE__, __LINE__};
    }
    ret.resize(outLen);
    outLen = ::MultiByteToWideChar(CP_UTF8, kFlags, a.data(), inputLen, &ret[0], ret.length());
    if (outLen == 0)
    {
        
        const DWORD error = ::GetLastError();
        throw SystemException{error, __FILE__, __LINE__};
    }
    return ret;
}

std::string to_string(std::wstring a)
{
    std::string ret;
    if (a.empty())
    {
        return ret;
    }
    const DWORD kFlags = WC_ERR_INVALID_CHARS;
    if (a.length() > std::numeric_limits<size_t>::max())
    {
        throw std::overflow_error("Input string too long: length greater than size_t can hold.");
    }
    int inputLen = static_cast<int>(a.length());
    int outLen = ::WideCharToMultiByte(CP_UTF8, 0, a.c_str(), -1,nullptr, 0, nullptr, nullptr);
    if (outLen == 0)
    {
        const DWORD error = ::GetLastError();
        throw SystemException{error, __FILE__, __LINE__};
    }
    ret.resize(outLen);
    outLen = ::WideCharToMultiByte(CP_UTF8, 0, a.c_str(),a.length() , &ret[0], ret.length(), nullptr, nullptr);
    if (outLen == 0)
    {
        
        const DWORD error = ::GetLastError();
        throw SystemException(error, __FILE__, __LINE__);
    }
    return ret;
}


namespace Utility
{

class PerformanceCounter
{
	LARGE_INTEGER startTime, EndTime, ElapsedMicroseconds,Frequency;
public:
	explicit PerformanceCounter() noexcept
	{
		::QueryPerformanceFrequency(&Frequency);
		::QueryPerformanceCounter(&startTime);
	}


	LARGE_INTEGER GetElapsedMicroSeconds() noexcept
	{
		::QueryPerformanceCounter(&EndTime);
		ElapsedMicroseconds.QuadPart = EndTime.QuadPart - startTime.QuadPart;
		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
		return ElapsedMicroseconds;
	}
};


template <typename Traits> class unique_handle
{
    typedef typename Traits::pointer pointer;

    pointer m_value;
    unique_handle<Traits>(unique_handle<Traits> const &) = delete;
	unique_handle<Traits> operator=(unique_handle<Traits> const &) = delete;
    void close() throw()
    {
        if (*this)
        {
            Traits::close(m_value);
        }
    }

  public:
    unique_handle<Traits>(unique_handle<Traits>&&other) throw() : m_value{other.release()}
    {
    }

    auto swap(unique_handle<Traits> &other) throw() -> void
    {
        std::swap(m_value, other.m_value);
    }
	
	
	unique_handle<Traits> operator=(unique_handle<Traits> &&other)
    {
        if (this != &other)
        {
            reset(other.release());
        }
        return *this;
    }

    auto get() const throw() -> pointer
    {
        return m_value;
    }

    auto release() throw() -> pointer
    {
        auto value = m_value;
        m_value = Traits::invalid();
        return value;
    }

    auto reset(pointer value = Traits::invalid()) throw() -> bool
    {
        if (m_value != value)
        {
            close();
            m_value = value;
        }
        return static_cast<bool>(*this);
    }

    explicit operator bool() const throw()
    {
        return m_value != Traits::invalid();
    }


    explicit unique_handle<Traits>(pointer value = Traits::invalid()) throw() : m_value{value}
    {
    }

    ~unique_handle() throw()
    {
        close();
    }
};

template <typename Traits>
bool operator==(unique_handle<Traits> const &left, unique_handle<Traits> const &right) noexcept 
{
    return left.get() == right.get();
}

template <typename Traits>
bool operator!=(unique_handle<Traits> const &left, unique_handle<Traits> const &right) noexcept
{
    return left.get() < right.get();
}

template <typename Traits>
bool operator>=(unique_handle<Traits> const &left, unique_handle<Traits> const &right) noexcept
{
    return left.get() >= right.get();
}

template <typename Traits>
bool operator>(unique_handle<Traits> const &left, unique_handle<Traits> const &right) noexcept
{
    return left.get() > right.get();
}

template <typename Traits>
bool operator<=(unique_handle<Traits> const &left, unique_handle<Traits> const &right) noexcept
{
    return left.get() <= right.get();
}

template <typename Traits>
bool operator<(unique_handle<Traits> const &left, unique_handle<Traits> const &right) noexcept
{
    return left.get() < right.get();
}

template <typename Traits> 
void swap(unique_handle<Traits> &left, unique_handle<Traits> &right) noexcept
{
    left.swap(right);
}

struct null_handle_traits
{
    typedef HANDLE pointer;
    pointer invalid() noexcept
    {
        return nullptr;
    }
    static void close(pointer value) noexcept
    {      
        VERIFY(CloseHandle(value));
    }
};
typedef unique_handle<null_handle_traits> null_handle;

struct invalid_handle_traits
{
    typedef HANDLE pointer;

    static auto invalid() throw() -> pointer
    {
        return INVALID_HANDLE_VALUE;
    }

    static auto close(pointer value) throw() -> void
    {
        CloseHandle(value);
    }
};

struct invalid_key_traits
{
	typedef HKEY pointer;

	static pointer invalid() noexcept 
	{
		return nullptr;
	}

	static void close(pointer value)
	{
		auto retCode = ::RegCloseKey(value);
		if(retCode != ERROR_SUCCESS)
		{
			throw System::SystemException{retCode,__FILE__,__LINE__};
		}
	}
};


typedef unique_handle<invalid_handle_traits> invalid_handle;

struct map_view_deleter
{
    typedef char const *pointer;

    auto operator()(pointer value) const throw() -> void
    {
        VERIFY(UnmapViewOfFile(value));
    }
};

auto trim(std::string const &s) -> std::string
{

    auto front = std::find_if_not(std::begin(s), std::end(s), isspace);
    auto back = std::find_if_not(s.rbegin(), s.rend(), isspace);
    return std::string{front, back.base()};
}

auto trim(std::wstring const &s) -> std::wstring
{
    auto front = std::find_if_not(std::begin(s), std::end(s), iswspace);
    auto back = std::find_if_not(s.rbegin(), s.rend(), iswspace);
    return std::wstring{front, back.base()};
}


auto split(std::tstring const &s, std::vector<std::tstring> delims) -> std::vector<std::tstring>
{
    size_t last = 0;
    size_t next = 0;
    std::vector<std::tstring> ret;
    std::tstring delimiter;

    if (delims.size() > 1)
    {
		// find the first delimiter in the string from list of delimiters
        delimiter = *std::min_element(delims.begin(), delims.end(),
                                      [&](auto a, auto b) { return s.find(a, last) < s.find(b, last); });
    }
    else
    {
		//there's only one delimiter, use it.
        delimiter = delims[0];
    }
    while ((next = s.find(delimiter, last)) != std::string::npos)
    {
        ret.push_back(s.substr(last, next - last));
        last = next + 1;
        // find the next delimiter in the string from list of delimiters
        if (delims.size() > 1)
        {
            delimiter = *std::min_element(delims.begin(), delims.end(),
                                          [&](auto a, auto b) { return s.find(a, last) < s.find(b, last); });
        }
    }
    return ret;
}

} // namespace Utility


class Guid
{
	std::unique_ptr<UUID> m_guid;
public:
	explicit Guid()
	{		
		m_guid = std::make_unique<UUID>();
		auto retCode = ::UuidCreate(m_guid.get());
		if(!((retCode == RPC_S_OK) || (retCode == RPC_S_UUID_LOCAL_ONLY)))
		{
			//if this happens there's a bigger issue than trying to create a 36 character string
			throw std::bad_alloc{};
		} 
	}

	Guid(std::tstring val) 
	{
	#ifdef UNICODE
		RPC_STATUS retCode = ::UuidFromStringW((RPC_WSTR)val.c_str(),m_guid.get());
	#else
		RPC_STATUS retCode = ::UuidFromStringA((RPC_CSTR)val.c_str(),m_guid.get());
	#endif
		if(retCode == RPC_S_INVALID_STRING_UUID)
		{
			throw std::runtime_error("invalid string");
		}
	}	

	std::tstring ToString()
	{
		if(m_guid)
		{	
				#ifdef UNICODE
				RPC_WSTR str;
				#else
				RPC_CSTR str;
				#endif

				auto status = ::UuidToString(m_guid.get(),&str);
				if(status != RPC_S_OK)
				{
					throw std::bad_alloc();
				}
				std::tstring ret = (TCHAR *)str;
				//ret.resize(37);
				//std::strcpy(&ret[0],(TCHAR *)str);				
				::RpcStringFree(&str);
				return ret;				
		}
	}
	UUID Get()
	{
		return *m_guid.get();
	}
};

class Transaction
{
	HANDLE m_trx;
public:
	explicit Transaction(HANDLE trx) noexcept : m_trx{trx}
	{

	}
	
	Transaction(Transaction&& other) noexcept : m_trx{other.m_trx}
	{
		other.m_trx = nullptr;
	}

	Transaction& operator=(Transaction&& other) noexcept
	{
		if((this != &other) && (m_trx != other.m_trx))
		{
			Close();
			m_trx = other.m_trx;
			other.m_trx = nullptr;
		}
	}
    
	Transaction(LPSECURITY_ATTRIBUTES lpTransactionAttributes,DWORD CreateOptions, DWORD IsolationLevel, DWORD IsolationFlags, DWORD Timeout,const std::tstring& Description)
	{
		m_trx = ::CreateTransaction(lpTransactionAttributes,
									nullptr,
									CreateOptions,
									IsolationLevel,
									IsolationFlags,
									Timeout,
									#ifndef UNICODE
									const_cast<wchar_t *>(System::to_wstring(Description).c_str()));
									#else
									const_cast<wchar_t *>(Description.c_str()));
									#endif
		if(m_trx == INVALID_HANDLE_VALUE)
		{
			throw SystemException(::GetLastError(),__FILE__, __LINE__);			
		}
	}



	void Close()
	{
		if(IsValid())
		{
			if(!CloseHandle(m_trx))
			{
				throw SystemException(::GetLastError(), __FILE__,__LINE__);
			}
		}
		m_trx = nullptr;
	}

	bool IsValid()
	{	
		if(m_trx != nullptr)
		{
			return true;
		}					
		return false;
	}

	~Transaction()	
	{
		Close();
	}

};

//winreg.h
class RegistryKey
{	
	HKEY m_key;

	void SetValue(std::tstring subKeyName, std::tstring valueName, DWORD dataType, const std::vector<byte>& data)
	{
		auto retCode = ::RegSetKeyValue(m_key,
			subKeyName.c_str(),
			valueName.c_str(),
			dataType,
			&data[0],
			data.size());
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{ retCode, __FILE__, __LINE__ };
		}
	}

public:
	RegistryKey() noexcept  = default;
	
	explicit RegistryKey(HKEY key) noexcept : m_key{key}
	{
		
	}
	
	RegistryKey(RegistryKey&& other) noexcept : m_key{other.m_key}
	{
		other.m_key = nullptr;
	}
	
	RegistryKey& operator=(RegistryKey&& other) noexcept
	{
		if((this != &other) && (m_key != other.m_key))
		{
			Close();
			
			m_key = other.m_key;
			other.m_key = nullptr;
		}
		return *this;
	}
	
	RegistryKey(const RegistryKey&) = delete;
	RegistryKey& operator=(const RegistryKey&) = delete;
	
	~RegistryKey() noexcept
	{
		Close();
	}
	
	HKEY Get() const noexcept
	{
		return m_key;
	}
	
	bool IsValid() const noexcept
	{
		return m_key != nullptr;
	}
	
	void Close() noexcept
	{
		if(IsValid())
		{
		  ::RegCloseKey(m_key);
		  	m_key = nullptr;
		}		
	}
	
	explicit operator bool() const noexcept
	{
		return IsValid();	
	}
	
	HKEY Detach() noexcept
	{
		HKEY hKey = m_key;
		m_key = nullptr;
		return hKey;
	}
	
	void Attach(HKEY hKey) noexcept
	{
		if(m_key != hKey)
		{
			Close();
			m_key = hKey;
		}
	}
	
	friend void swap(RegistryKey& a, RegistryKey& b) noexcept
	{
		using std::swap;
		swap(a.m_key, b.m_key);
	}
	
	friend bool operator==(const RegistryKey& a, const RegistryKey& b) noexcept
	{
		return a.m_key == b.m_key;
	}
	
	friend bool operator!=(const RegistryKey& a, const RegistryKey& b) noexcept
	{
		return a.m_key != b.m_key;
	}
	
	friend bool operator<(const RegistryKey& a, const RegistryKey& b) noexcept
	{
		return a.m_key < b.m_key;
	}
    
	friend bool operator>(const RegistryKey& a, const RegistryKey& b) noexcept
	{
		return a.m_key > b.m_key;
	}
	
	friend bool operator>=(const RegistryKey& a, const RegistryKey& b) noexcept
	{
		return a.m_key >= b.m_key;
	}
		
	DWORD GetDword(const std::tstring& subkey, const std::tstring& value)
	{
		DWORD data{};
		DWORD dataSize = sizeof(DWORD);
		LONG retCode = ::RegGetValue(m_key,subkey.c_str(),value.c_str(),RRF_RT_REG_DWORD,nullptr,&data, &dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		return data;
	}
	
	ULONGLONG GetQword(const std::tstring& subKey, const std::tstring& value)
	{
		ULONGLONG data{};
		DWORD dataSize = sizeof(data);
		LONG retCode = ::RegGetValue(
			m_key,
			subKey.c_str(),
			value.c_str(),
			RRF_RT_REG_QWORD,
			nullptr,
			&data,
			&dataSize);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
	
		return data;
	}
	
	std::tstring GetString(const std::tstring& subKey, const std::tstring& value)
	{
		DWORD dataSize{};
		LONG retCode = :: RegGetValue(m_key,subKey.c_str(),value.c_str(),RRF_RT_REG_SZ,nullptr,nullptr,&dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		
		std::tstring data;
		data.resize(dataSize / sizeof(TCHAR)); 
		
		retCode = ::RegGetValue(m_key,subKey.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr,&data[0],&dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		
		data.resize((dataSize / sizeof(TCHAR)) - 1); //chop off the extra 0;
		
		return data;
	}
	
	std::vector<std::tstring> GetMultiString(const std::tstring& subKey, const std::tstring& value)
	{
		DWORD dataSize{};
		LONG retCode = ::RegGetValue(m_key,subKey.c_str(), value.c_str(), REG_MULTI_SZ, nullptr, nullptr, &dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		dataSize += sizeof(TCHAR);
		std::vector<TCHAR> data;
		data.resize(dataSize / sizeof(TCHAR));
		
		retCode = ::RegGetValue(m_key, subKey.c_str(), value.c_str(), REG_MULTI_SZ, nullptr, &data[0], &dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		
		std::vector<std::tstring> result;
		const TCHAR* currPtr = &data[0];
		while(*currPtr != *(TEXT("\0")))
		{
			const size_t currentStringLen = _tcslen(currPtr);
			result.emplace_back(std::tstring{currPtr, currentStringLen});
			currPtr += currentStringLen +1;
		}
		
		return result;
	}
	
	std::vector<byte> GetBinary(const std::tstring& subKey, const std::tstring& value)
	{
		DWORD dataSize{};
		LONG retCode = ::RegGetValue(
			m_key,
			subKey.c_str(),
			value.c_str(),
			REG_BINARY,
			nullptr,
			nullptr,
			&dataSize);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		
		std::vector<byte> data;
		data.resize(dataSize);
		
		retCode = ::RegGetValue(
        m_key,
        subKey.c_str(),
        value.c_str(),
        REG_BINARY,
        nullptr,
        &data[0],
        &dataSize);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		
		return data;
	}
	


	void DeleteValue(const std::tstring& valueName)
	{
		auto retCode = ::RegDeleteValue(m_key,
										valueName.c_str());
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode, __FILE__, __LINE__};
		}
	}

	void SetQWORD(std::tstring valueName, ULONGLONG value)
	{
		std::vector<byte> data;
		data.resize(sizeof(ULONGLONG));
		memcpy(&data[0],&value,data.size());
		SetValue(TEXT(""),valueName,REG_QWORD, data);
	}

	void SetString(std::tstring valueName, const std::tstring& valueData)
	{
		std::vector<byte> data;
		auto totalCharacters = (valueData.size() + 1);
		auto charSizeinBytes = (sizeof(TCHAR) / sizeof(byte);
		data.resize(totalCharacters * charSizeinBytes);
		memcpy(&data[0], &valueData[0], data.size());
		SetValue(TEXT(""), valueName, REG_SZ, data);
	}

	void SetBinary(std::tstring valueName, const std::vector<byte>& data)
	{
		SetValue(TEXT(""), valueName, REG_BINARY, data);
	}
	
	void SetDWORD(std::tstring valueName, const DWORD& value)
	{
		std::vector<byte> data;
		data.resize(sizeof(DWORD));
		memcpy( &data[0], &value, data.size());
		SetValue(TEXT(""), valueName, REG_DWORD, data);
	}

	//This is ; delimited path information, type REG_EXPAND_SZ because it can contain special folder names, like %USERPROFILE%. 
	void SetPath(std::tstring valueName, const std::tstring& pathData)
	{
		std::vector<byte> data;
		data.resize((pathData.size() * (sizeof(TCHAR) / sizeof(byte))) + 1);
		memcpy(&data[0], &pathData[0], data.size());
		SetValue(TEXT(""), valueName, REG_EXPAND_SZ, data);
	}
	
	void SetMultiString(std::tstring valueName, const std::vector<std::tstring>& multiData)
	{
		size_t totalChars = std::accumulate(multiData.begin(), multiData.end(), 0, [](auto a, auto b) { return a + b.size(); });
		std::vector<byte> data;
		auto nullterminators = (multiData.size() + 1) ; //each of the strings + 1 extra to mark the end.
		totalChars += nullterminators;
		auto totalCharsInBytes = totalChars * (sizeof(TCHAR) / sizeof(byte)); //maybe 1, maybe 2. 
		data.resize(totalCharsInBytes);
		size_t pos = 0;
		for (auto a : multiData)
		{
			size_t inBytes = (a.size() * sizeof(TCHAR));
			memcpy(&data[pos], &a[0], inBytes);
			pos += inBytes + sizeof(TCHAR); //+1 for the null terminator. 
		}

		SetValue(TEXT(""), valueName, REG_MULTI_SZ, data);
	}

	std::vector<std::pair<std::tstring, DWORD>> GetValuesInfo()
	{
		DWORD valueCount{};
		DWORD maxValueNameLen{};
		LONG retCode = ::RegQueryInfoKey(m_key,
										 nullptr,
										 nullptr,
										 nullptr,
										 nullptr,
										 nullptr,
										 nullptr,
										 &valueCount,
										 &maxValueNameLen,
										 nullptr,
										 nullptr,
										 nullptr
										 );
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		
		maxValueNameLen++; //incriment to include the terminating null.
		
		auto nameBuffer = std::make_unique<TCHAR[]>(maxValueNameLen);
		std::vector<std::pair<std::tstring, DWORD>> values;
		for(DWORD index = 0; index < valueCount; index++)
		{
			DWORD valueNameLen = maxValueNameLen;
			DWORD valueType{};
			retCode = ::RegEnumValue(m_key,
								     index,
									 nameBuffer.get(),
									 &valueNameLen,
									 nullptr,
									 &valueType,
									 nullptr,
									 nullptr
									 );
		    if(retCode != ERROR_SUCCESS)
			{
				throw SystemException{retCode,__FILE__, __LINE__};
			}
			
			values.push_back(std::make_pair(std::tstring{nameBuffer.get(),valueNameLen},valueType));
		}
		
		return values;
		
	}
	
	std::vector<std::pair<std::tstring,SYSTEMTIME>> GetSubKeys()
	{
		DWORD keyCount{};
		DWORD maxValueNameLen{};
		LONG retCode = ::RegQueryInfoKey(m_key,
										 nullptr,
										 nullptr,
										 nullptr,
										 &keyCount,
										 &maxValueNameLen,
										 nullptr,
										 nullptr,
										 nullptr,
										 nullptr,
										 nullptr,
										 nullptr
										 );
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		maxValueNameLen++; //for terminating 0. 
		std::vector<std::pair<std::tstring, SYSTEMTIME>> ret;
		
		for(DWORD index = 0; index < keyCount; index++)
		{
			std::tstring stname;
			stname.resize(maxValueNameLen);
			DWORD retNamelen = maxValueNameLen;
			FILETIME ft;
			SYSTEMTIME stUTC, stLocal;
			retCode = ::RegEnumKeyEx(m_key, index, &stname[0], &retNamelen,nullptr,nullptr, nullptr,&ft);
			if(retCode != ERROR_SUCCESS)
			{
				throw SystemException(retCode, __FILE__, __LINE__);
			}

			::FileTimeToSystemTime(&ft, &stUTC);
			::SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);
			auto name = stname.c_str();
			ret.emplace_back(std::make_pair(name,stLocal));
		}
		return ret;
	}

	RegistryKey CreateSubKey(const std::tstring& keyName, const std::tstring& keyClass, DWORD options, REGSAM access, SECURITY_ATTRIBUTES* securityAttributes)
	{
		HKEY hKey = nullptr;
		LONG retCode = ::RegCreateKeyEx(
										 m_key,
										 keyName.c_str(),
										 0,          // reserved
										 const_cast<LPTSTR>(keyClass.c_str()),
										 options,
										 access,
										 securityAttributes,
										 &hKey,
										 nullptr
										);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		
		return RegistryKey{ hKey };
	}
#if defined(_M_X64) || defined(__x86_64__)
	typename std::enable_if<System::Information::is64Bit::value>::type DeleteKey()
	{		
		//delete it's subkeys and values recusively. 
		auto retCode = ::RegDeleteTree(m_key, nullptr);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{ retCode,__FILE__, __LINE__ };
		}					

	}
#else
	typename std::enable_if<!System::Information::is64Bit::value>::type DeleteKey()
	{
		//delete it's subkeys and values recusively. 
		auto retCode = ::RegDeleteTree(m_key, nullptr);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{ retCode,__FILE__, __LINE__ };
		}

	}
#endif
#if defined(_M_X64) || defined(__x86_64__)

	void DeleteSubKey(std::tstring subKeyName)
	{
		//delete it's subkeys and values recusively. 
		auto retCode = ::RegDeleteTree(m_key, subKeyName.c_str());
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{ retCode,__FILE__, __LINE__ };
		}

	}
#else
	void DeleteSubKey(std::tstring subKeyName)
	{
		//delete it's subkeys and values recusively. 
		auto retCode = ::RegDeleteTree(m_key, subKeyName.c_str());
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{ retCode,__FILE__, __LINE__ };
		}

	}
#endif
	
	RegistryKey(HKEY hKeyParent, const std::tstring& keyName, REGSAM access)
	{
		HKEY hKey = nullptr;
		LONG retCode = ::RegOpenKeyEx(
        hKeyParent,
        keyName.c_str(),
        0, // default options
        access,
        &m_key
		);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
	}
	
	RegistryKey OpenKey(const std::tstring& keyName, REGSAM access)
	{
		HKEY hKey = nullptr;
		LONG retCode = ::RegOpenKeyEx(
        m_key,
        keyName.c_str(),
        0, // default options
        access,
        &hKey
		);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode,__FILE__, __LINE__};
		}
		return RegistryKey {hKey};
	}

	//HKEY_LOCAL_MACHINE, HKEY_PERFORMANCE_DATA or HKEY_USERS only. 
	static RegistryKey ConnectToRemoteRegistry(std::tstring machineName, HKEY hiveName)
	{
		HKEY hRet{};
		DWORD retCode = ::RegConnectRegistry(machineName.c_str(),hiveName, &hRet);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode, __FILE__, __LINE__};
		}
		return RegistryKey {hRet};
	}

	void CopyTree(std::tstring subKey, RegistryKey& keyDest)
	{
		DWORD retCode = ::RegCopyTree(m_key,subKey.c_str(),keyDest.Get());
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{retCode, __FILE__,__LINE__};
		}
	}

};


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
} // namespace Debug

} // namespace System
#endif
#endif