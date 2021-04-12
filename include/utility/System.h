
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
namespace System
{

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


std::wstring to_wstring(std::string a);
std::string to_string(std::wstring a);

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

#ifdef __GNUC__
#if __cplusplus <= 201103L
namespace std
{
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
} // namespace std
#endif
#endif

namespace std
{
#ifdef UNICODE
	typedef std::wstring tstring;
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
	typedef std::string tstring;
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
}

namespace System
{



class SystemException 
{
	std::tstring localbuff;
    PTSTR buffer;
	const char * m_file;
	int linenum;
 public:
    DWORD m_val;
	HRESULT m_hval;	
	
    SystemException(DWORD val, const char* file, int line) : m_val(val), m_hval(0), buffer(nullptr), m_file(file), linenum(line)
    {
    }


    PCTSTR what()
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
	
    ~SystemException()
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



class RegistryKey
{
	HKEY m_key;
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
	
	
	DWORD RegGetDword(const std::tstring& subkey, const std::tstring& value)
	{
		DWORD data{};
		DWORD dataSize = sizeof(DWORD);
		LONG retCode = ::RegGetValue(m_key,subkey.c_str(),value.c_str(),RRF_RT_REG_DWORD,nullptr,&data, &dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
	}
	
	ULONGLONG RegGetQword(const std::tstring& subKey, const std::tstring& value)
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
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
	
		return data;
	}
	
	std::tstring RegGetString(const std::tstring& subKey, const std::tstring& value)
	{
		DWORD dataSize{};
		LONG retCode = :: RegGetValue(m_key,subKey.c_str(),value.c_str(),RRF_RT_REG_SZ,nullptr,nullptr,&dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
		
		std::tstring data;
		data.resize(dataSize / sizeof(TCHAR)); 
		
		retCode = ::RegGetValue(m_key,subKey.c_str(), value.c_str(), RRF_RT_REG_SZ, nullptr,&data[0],&dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
		
		data.resize((dataSize / sizeof(TCHAR)) - 1); //chop off the extra 0;
		
		return data;
	}
	
	std::vector<std::tstring> RegGetMultiString(const std::tstring& subKey, const std::tstring& value)
	{
		DWORD dataSize{};
		LONG retCode = ::RegGetValue(m_key,subKey.c_str(), value.c_str(), RRF_RT_REG_MULTI_SZ, nullptr, nullptr, &dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
		
		std::vector<TCHAR> data;
		data.resize(dataSize / sizeof(TCHAR));
		
		retCode = ::RegGetValue(m_key, subKey.c_str(), value.c_str(), RRF_RT_REG_MULTI_SZ, nullptr, &data[0], &dataSize);
		if(retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
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
	
	std::vector<byte> RegGetBinary(const std::tstring& subKey, const std::tstring& value)
	{
		DWORD dataSize{};
		LONG retCode = ::RegGetValue(
			m_key,
			subKey.c_str(),
			value.c_str(),
			RRF_RT_REG_BINARY,
			nullptr,
			nullptr,
			&dataSize);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
		
		std::vector<byte> data;
		data.resize(dataSize);
		
		retCode = ::RegGetValue(
        m_key,
        subKey.c_str(),
        value.c_str(),
        RRF_RT_REG_BINARY,
        nullptr,
        &data[0],
        &dataSize);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
		
		return data;
	}
	
	std::vector<std::pair<std::tstring, DWORD>> RegEnumValues()
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
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
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
				throw SystemException{::GetLastError(),__FILE__, __LINE__};
			}
			
			values.push_back(std::make_pair(std::tstring{nameBuffer.get(),valueNameLen},valueType));
		}
		
		return values;
		
	}
	
	RegistryKey RegCreateKey(HKEY hKeyParent, const std::tstring& keyName, const std::tstring& keyClass, DWORD options, REGSAM access, SECURITY_ATTRIBUTES* securityAttributes, DWORD* disposition)
	{
		HKEY hKey = nullptr;
		LONG retCode = ::RegCreateKeyEx(
        hKeyParent,
        keyName.c_str(),
        0,          // reserved
        const_cast<LPTSTR>(keyClass.c_str()),
        options,
        access,
        securityAttributes,
        &hKey,
        disposition
		);
		if (retCode != ERROR_SUCCESS)
		{
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}			
		return RegistryKey{ hKey };
	}
	
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
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
	}
	
	void OpenKey(HKEY hKeyParent, const std::tstring& keyName, REGSAM access)
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
			throw SystemException{::GetLastError(),__FILE__, __LINE__};
		}
	}
};

namespace Utility
{

template <typename Traits> class unique_handle
{
    typedef typename Traits::pointer pointer;

    pointer m_value;
    unique_handle(unique_handle const &);
    auto operator=(unique_handle const &) -> unique_handle;
    auto close() throw() -> void
    {
        if (*this)
        {
            Traits::close(m_value);
        }
    }

  public:
    unique_handle(unique_handle &&other) throw() : m_value{other.release()}
    {
    }

    auto swap(unique_handle<Traits> &other) throw() -> void
    {
        std::swap(m_value, other.m_value);
    }

    auto operator=(unique_handle &&other) throw() -> unique_handle
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

    explicit unique_handle(pointer value = Traits::invalid()) throw() : m_value{value}
    {
    }

    ~unique_handle() throw()
    {
        close();
    }
};

template <typename Traits>
auto operator==(unique_handle<Traits> const &left, unique_handle<Traits> const &right) throw() -> bool
{
    return left.get() == right.get();
}

template <typename Traits>
auto operator!=(unique_handle<Traits> const &left, unique_handle<Traits> const &right) throw() -> bool
{
    return left.get() < right.get();
}

template <typename Traits>
auto operator>=(unique_handle<Traits> const &left, unique_handle<Traits> const &right) throw() -> bool
{
    return left.get() >= right.get();
}

template <typename Traits>
auto operator>(unique_handle<Traits> const &left, unique_handle<Traits> const &right) throw() -> bool
{
    return left.get() > right.get();
}

template <typename Traits>
auto operator<=(unique_handle<Traits> const &left, unique_handle<Traits> const &right) throw() -> bool
{
    return left.get() <= right.get();
}

template <typename Traits>
auto operator<(unique_handle<Traits> const &left, unique_handle<Traits> const &right) throw() -> bool
{
    return left.get() < right.get();
}

template <typename Traits> auto swap(unique_handle<Traits> &left, unique_handle<Traits> &right) throw() -> void
{
    left.swap(right);
}

struct null_handle_traits
{
    typedef HANDLE pointer;
    static auto invalid() throw() -> pointer
    {
        return nullptr;
    }
    static auto close(pointer value) throw() -> void
    {
        // wrap in verify
        CloseHandle(value);
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

#ifdef __SYSTEM__
auto split(std::tstring const &s, std::vector<std::tstring> delims) -> std::vector<std::tstring>
{
    size_t last = 0;
    size_t next = 0;
    std::vector<std::tstring> ret;
    std::tstring delimiter;

    if (delims.size() > 1)
    {
        delimiter = *std::min_element(delims.begin(), delims.end(),
                                      [&](auto a, auto b) { return s.find(a, last) < s.find(b, last); });
    }
    else
    {
        delimiter = delims[0];
    }
    while ((next = s.find(delimiter, last)) != std::string::npos)
    {
        ret.push_back(s.substr(last, next - last));
        last = next + 1;
        // find the next delimiter in the list of delimiters
        if (delims.size() > 1)
        {
            delimiter = *std::min_element(delims.begin(), delims.end(),
                                          [&](auto a, auto b) { return s.find(a, last) < s.find(b, last); });
        }
    }
    return ret;
}

#endif

}; // namespace Utility


#ifdef __GNUC__
#if __cplusplus <= 201103L
namespace std
{
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
} // namespace std
#endif
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
#endif