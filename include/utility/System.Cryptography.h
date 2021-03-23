#ifndef __SYSTEM_CRYTPOGRPAHY__
#define __SYSTEM_CRYTPOGRPAHY__

#ifndef __SYSTEM__
#include "System.h"
#endif
#ifndef __SYSTEM_UTILITY__
#include "System.Utility.h"
#endif

#ifndef __GNUG__
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "Crypt32.lib")
#endif

#include <WinCrypt.h>
#include <bcrypt.h>

#include <assert.h>
#include <type_traits>

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ASSERT(SUCCEEDED(Status))
#endif

namespace ___detail
{

struct MD2Type
{
};
struct MD4Type
{
};
struct MD5Type
{
};
struct SHA1Type
{
};
struct SHA256Type
{
};
struct SHA386Type
{
};
struct SHA512Type
{
};

struct AESType
{
};
#if _WIN32_WINNT == _WIN32_WINNT_WIN8
struct AES_CMAC_Type
{
};
#endif
// struct AES_GMAC_Type{};
struct DESType
{
};
struct DES3Type
{
};
struct RC2Type
{
};
struct RC4Type
{
};
} // namespace ___detail

namespace System
{
namespace Debug
{
inline void PrintError(NTSTATUS NTStatusMessage)
{
    if (!SUCCEEDED(NTStatusMessage))
    {
        LPVOID lpMessageBuffer;
        HMODULE Hand = LoadLibrary(TEXT("NTDLL.DLL"));

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE, Hand,
                      (DWORD)NTStatusMessage, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessageBuffer, 0,
                      NULL);

        // Now display the string.
        OutputDebugString((LPTSTR)lpMessageBuffer);
        // Free the buffer allocated by the system.
        LocalFree(lpMessageBuffer);
        FreeLibrary(Hand);
    }
}
struct nt_error
{
    std::basic_string<LPTSTR> error_text;
    NTSTATUS error;
    nt_error(NTSTATUS stat)
    {
        error = stat;
        if (!SUCCEEDED(stat))
        {
            LPVOID lpMessageBuffer;
            HMODULE Hand = LoadLibrary(TEXT("NTDLL.DLL"));

            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE,
                          Hand, (DWORD)stat, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessageBuffer, 0,
                          NULL);

            error_text = reinterpret_cast<LPTSTR>(lpMessageBuffer);
            // Free the buffer allocated by the system.
            LocalFree(lpMessageBuffer);
            FreeLibrary(Hand);
        }
    }
};
}; // namespace Debug
namespace Utility
{

struct bcrypt_provider_traits
{
    typedef BCRYPT_ALG_HANDLE pointer;

    static auto invalid() throw() -> pointer
    {
        return nullptr;
    }
    static auto close(pointer value) throw() -> void
    {
        if (value != invalid())
        {
            NTVERIFY(BCryptCloseAlgorithmProvider(value, 0));
        }
    }
};

struct bcrypt_hash_traits
{
    typedef BCRYPT_HASH_HANDLE pointer;
    static auto invalid() throw() -> pointer
    {
        return nullptr;
    }
    static auto close(pointer value) throw() -> void
    {
        NTVERIFY(BCryptDestroyHash(value));
    }
};

struct bcrypt_key_traits
{
    typedef BCRYPT_KEY_HANDLE pointer;
    static auto invalid() throw() -> pointer
    {
        return nullptr;
    }
    static auto close(pointer value) throw() -> void
    {
        if (value)
        {
            NTVERIFY(BCryptDestroyKey(value));
        }
    }
};
}; // namespace Utility

namespace Crypto
{
#ifndef CRYPT_STRING_NOCRLF
#define CRYPT_STRING_NOCRLF 0x40000000
#endif
#ifndef CRYPT_STRING_HEXRAW
#define CRYPT_STRING_HEXRAW 0x0000000c
#endif
#ifndef CRYPT_STRING_STRICT
#define CRYPT_STRING_STRICT 0x20000000
#endif

enum class CryptStringType : DWORD
{
    Base64Header = CRYPT_STRING_BASE64HEADER,
    Base64HeaderNoCr = CRYPT_STRING_BASE64HEADER | CRYPT_STRING_NOCR,
    Base64HeaderNoCrLf = CRYPT_STRING_BASE64HEADER | CRYPT_STRING_NOCRLF,
    Base64 = CRYPT_STRING_BASE64,
    Base64NoCr = CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR,
    Base64NoCrLf = CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR,
    Biniary = CRYPT_STRING_BINARY,
    BiniaryNoCr = CRYPT_STRING_BINARY | CRYPT_STRING_NOCR,
    BiniaryNoCrLf = CRYPT_STRING_BINARY | CRYPT_STRING_NOCRLF,
    Base64WithRequestHeader = CRYPT_STRING_BASE64REQUESTHEADER,
    Base64WithRequestHeaderNoCr = CRYPT_STRING_BASE64REQUESTHEADER | CRYPT_STRING_NOCR,
    Base64WithRequestheaderNoCrLf = CRYPT_STRING_BASE64REQUESTHEADER | CRYPT_STRING_NOCRLF,
    Hex = CRYPT_STRING_HEX,
    HexNoCr = CRYPT_STRING_HEX | CRYPT_STRING_NOCR,
    HexNoCrLf = CRYPT_STRING_HEX | CRYPT_STRING_NOCRLF,
    HexASCII = CRYPT_STRING_HEXASCII,
    HexASCIINoCr = CRYPT_STRING_HEXASCII | CRYPT_STRING_NOCR,
    HexASCIINoCrLF = CRYPT_STRING_HEXASCII | CRYPT_STRING_NOCRLF,
    Base64WithX509ClrHeader = CRYPT_STRING_BASE64X509CRLHEADER,
    Base64WithX509ClrHeaderNoCr = CRYPT_STRING_BASE64X509CRLHEADER | CRYPT_STRING_NOCR,
    Base64WithX509ClrHeaderNoCrLf = CRYPT_STRING_BASE64X509CRLHEADER | CRYPT_STRING_NOCRLF,
    HexWithAddress = CRYPT_STRING_HEXADDR,
    HexWithAddressNoCr = CRYPT_STRING_HEXADDR | CRYPT_STRING_NOCR,
    HexWithAddressNoCrLf = CRYPT_STRING_HEXADDR | CRYPT_STRING_NOCRLF,
    HexRaw = CRYPT_STRING_HEXRAW,
    HexRawNoCr = CRYPT_STRING_HEXRAW | CRYPT_STRING_NOCR,
    HexRawNoCrLf = CRYPT_STRING_HEXRAW | CRYPT_STRING_NOCRLF,
    HexASCIIWithAddress = CRYPT_STRING_HEXASCIIADDR,
    HexASCIIWithAddressNoCr = CRYPT_STRING_HEXASCIIADDR | CRYPT_STRING_NOCR,
    HexASCIIWithAddressNoCrlf = CRYPT_STRING_HEXASCIIADDR | CRYPT_STRING_NOCRLF,
    Strict = CRYPT_STRING_STRICT,
    StrictNoCr = CRYPT_STRING_STRICT | CRYPT_STRING_NOCR,
    StrictNoCrLf = CRYPT_STRING_STRICT | CRYPT_STRING_NOCRLF
};

inline std::wstring BinaryToStringW(CryptStringType sType, std::vector<byte> &pbData)
{
    std::wstring m_strCrypt;
    DWORD buffLen = 0; // DWORD buffer to hold the needed size of our string
    VERIFY(CryptBinaryToStringW(&pbData[0], static_cast<DWORD>(pbData.size()), static_cast<DWORD>(sType), nullptr,
                                &buffLen)); // Calculate the size needed for our string.
    m_strCrypt.resize(buffLen);             // Resize our buffer to fit the needed size
    VERIFY(CryptBinaryToStringW(&pbData[0], static_cast<DWORD>(pbData.size()), static_cast<DWORD>(sType),
                                &m_strCrypt[0],
                                &buffLen)); // Finally, fill our string buffer
                                            // with the hexadecimal representation
                                            // of our hash.
    return m_strCrypt;
}

inline std::string BinaryToStringA(CryptStringType sType, std::vector<byte> &pbData)
{
    std::string m_strCrypt;
    DWORD buffLen = 0; // DWORD buffer to hold the needed size of our string
    VERIFY(CryptBinaryToStringA(&pbData[0], static_cast<DWORD>(pbData.size()), static_cast<DWORD>(sType), nullptr,
                                &buffLen)); // Calculate the size needed for our string.

    m_strCrypt.resize(buffLen); // Resize our buffer to fit the needed size

    VERIFY(CryptBinaryToStringA(&pbData[0], static_cast<DWORD>(pbData.size()), static_cast<DWORD>(sType),
                                &m_strCrypt[0],
                                &buffLen)); // Finally, fill our string buffer
                                            // with the hexadecimal representation
                                            // of our hash.
    return m_strCrypt;
}

#ifdef UNICODE

inline std::wstring BinaryToString(CryptStringType sType, std::vector<byte> &pbData)
{
    return BinaryToStringW(sType, pbData);
}
#else
inline std::string BiniaryToString(CryptStringType sType, std::vector<byte> &pbData)
{
    return BinaryToStringA(sType, pbData);
}
#endif

namespace Hash
{

template <typename T> class HashImpl
{
    DWORD dwDataLen;
    DWORD dwHashLen;
    DWORD dwHashObjectLen;
    std::unique_ptr<byte[]> m_pbHashObject;
    std::unique_ptr<byte[]> m_pbHash;
    System::Utility::unique_handle<System::Utility::bcrypt_provider_traits> m_Provider;
    System::Utility::unique_handle<System::Utility::bcrypt_hash_traits> m_hash;
    HashImpl(HashImpl const &other);
    HashImpl operator=(HashImpl const &other);

  public:
    HashImpl() : dwDataLen(0), dwHashLen(0), dwHashObjectLen(0)
    {
        T t;
        ___HashImplHlpr(t);
    }
    HashImpl(HashImpl &&other) throw()
        : m_Provider{other.m_Provider.release()}, m_hash{other.m_hash.release()}, m_pbHash{other.m_pbHash.release()},
          m_pbHashObject{other.m_pbHashObject.release()}, dwDataLen{other.dwDataLen}, dwHashLen{other.dwHashLen}
    {
    }
    auto operator=(HashImpl &&other) throw() -> HashImpl
    {
        if (this != &other)
        {
            m_Provider = other.m_Provider.release();
            m_hash = other.m_hash.release();
            m_pbHash = other.m_pbHash.release();
            m_pbHashObject = other.m_pbHashObject.release();
            dwDataLen = other.dwDataLen;
            dwHashLen = other.dwDataLen;
        }
        return *this;
    }

  private:
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::MD5Type>::value, void>::type ___HashImplHlpr(R)
    {
        BCRYPT_ALG_HANDLE provider;
        NTVERIFY(BCryptOpenAlgorithmProvider(&provider, BCRYPT_MD5_ALGORITHM, MS_PRIMITIVE_PROVIDER,
                                             0)); // Initialize the algorithm provider handle
        m_Provider.reset(provider);               // Wrap the provider handle in a nice RAII class.
        ___Init();
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::MD4Type>::value, void>::type ___HashImplHlpr(R)
    {
        BCRYPT_ALG_HANDLE provider;
        NTVERIFY(BCryptOpenAlgorithmProvider(&provider, BCRYPT_MD4_ALGORITHM, MS_PRIMITIVE_PROVIDER,
                                             0)); // Initialize the algorithm provider handle
        m_Provider.reset(provider);               // Wrap the provider handle in a nice RAII class.
        ___Init();
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::MD2Type>::value, void>::type ___HashImplHlpr(R)
    {
        BCRYPT_ALG_HANDLE provider;
        NTVERIFY(BCryptOpenAlgorithmProvider(&provider, BCRYPT_MD2_ALGORITHM, MS_PRIMITIVE_PROVIDER,
                                             0)); // Initialize the algorithm provider handle
        m_Provider.reset(provider);               // Wrap the provider handle in a nice RAII class.
        ___Init();
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::SHA1Type>::value, void>::type ___HashImplHlpr(R)
    {
        BCRYPT_ALG_HANDLE provider;
        NTVERIFY(BCryptOpenAlgorithmProvider(&provider, BCRYPT_SHA1_ALGORITHM, MS_PRIMITIVE_PROVIDER,
                                             0)); // Initialize the algorithm provider handle
        m_Provider.reset(provider);               // Wrap the provider handle in a nice RAII class.
        ___Init();
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::SHA256Type>::value, void>::type ___HashImplHlpr(R)
    {
        BCRYPT_ALG_HANDLE provider;
        NTVERIFY(BCryptOpenAlgorithmProvider(&provider, BCRYPT_SHA256_ALGORITHM, MS_PRIMITIVE_PROVIDER,
                                             0)); // Initialize the algorithm provider handle
        m_Provider.reset(provider);               // Wrap the provider handle in a nice RAII class.
        ___Init();
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::SHA386Type>::value, void>::type ___HashImplHlpr(R)
    {
        BCRYPT_ALG_HANDLE provider;
        NTVERIFY(BCryptOpenAlgorithmProvider(&provider, BCRYPT_SHA384_ALGORITHM, MS_PRIMITIVE_PROVIDER,
                                             0)); // Initialize the algorithm provider handle
        m_Provider.reset(provider);               // Wrap the provider handle in a nice RAII class.
        ___Init();
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::SHA512Type>::value, void>::type ___HashImplHlpr(R)
    {
        BCRYPT_ALG_HANDLE provider;
        NTVERIFY(BCryptOpenAlgorithmProvider(&provider, BCRYPT_SHA512_ALGORITHM, MS_PRIMITIVE_PROVIDER,
                                             0)); // Initialize the algorithm provider handle
        m_Provider.reset(provider);               // Wrap the provider handle in a nice RAII class.
        ___Init();
    }

    void ___Init()
    {
        BCRYPT_HASH_HANDLE hash;
        NTVERIFY(BCryptGetProperty(m_Provider.get(), BCRYPT_OBJECT_LENGTH, reinterpret_cast<PBYTE>(&dwHashObjectLen),
                                   sizeof(DWORD), &dwDataLen,
                                   0)); // Calculate the hash object length so

        NTVERIFY(BCryptGetProperty(m_Provider.get(), BCRYPT_HASH_LENGTH, reinterpret_cast<PBYTE>(&dwHashLen),
                                   sizeof(DWORD), &dwDataLen,
                                   0)); // we can calculate the md5 hash length
                                        // (always 16 bytes, or 128 bit)

        /*
        Because the hash object, and the Hash it's self need to stick around for a
        while
        i wrap them in the std's unique_ptr class, it's more lightweight than using
        a vector<byte>
        and avoiding the use of new, and the unique_ptr class does provide exactly
        whats needed here.
        And, because new can fail, it's always better to do your allocations outside
        of assignment operations.
        */
        if (m_pbHash)
            m_pbHash.reset();
        if (m_pbHashObject)
            m_pbHash.reset();

        m_pbHashObject = std::make_unique<byte[]>(dwHashObjectLen);
        m_pbHash = std::make_unique<byte[]>(dwHashLen);

        NTVERIFY(BCryptCreateHash(m_Provider.get(), &hash, m_pbHashObject.get(), dwHashObjectLen, 0, 0,
                                  0)); // Initialize the hash handle

        m_hash.reset(hash); // Wrap the Hashing Algorithm handle in a
                            // System::Utility::unique_handle class
    }

  public:
    ~HashImpl(void)
    {
    }
    void AddDataToHash(std::vector<byte> data)
    {
        if (!m_hash)
            this->___Init();
        NTVERIFY(BCryptHashData(m_hash.get(), (PUCHAR)&data[0], (unsigned long)data.size(),
                                0)); // Add data to our hash.
    }

    std::vector<byte> Finalise()
    {

        NTVERIFY(BCryptFinishHash(m_hash.get(), reinterpret_cast<PUCHAR>(m_pbHash.get()), dwHashLen,
                                  0)); // Call finish on the hash object
                                       // when your done adding data to
                                       // be hashed to it. This places
        // the completed hash in our hash buffer. And because we need our hash in
        // nice string,
        // CryptBiniaryToString is the tool for the job.

        m_hash.reset();
        m_pbHashObject.reset();

        return std::vector<byte>(m_pbHash.get(), m_pbHash.get() + dwHashLen);
    }
};

typedef HashImpl<___detail::MD2Type> CBCryptMD2;
typedef HashImpl<___detail::MD4Type> CBCryptMD4;
typedef HashImpl<___detail::MD5Type> CBCryptMD5;
typedef HashImpl<___detail::SHA1Type> CBCryptSHA1;
typedef HashImpl<___detail::SHA256Type> CBCryptSHA256;
typedef HashImpl<___detail::SHA386Type> CBCryptSHA384;
typedef HashImpl<___detail::SHA512Type> CBCryptSHA512;
}; // namespace Hash

namespace Symmetric
{

template <typename T> class SymmetricImpl
{
    DWORD dwDataLen;
    DWORD dwKeyLen;
    DWORD dwIvLen;
    System::Utility::unique_handle<System::Utility::bcrypt_provider_traits> m_Provider;
    System::Utility::unique_handle<System::Utility::bcrypt_key_traits> m_Key;
    std::vector<byte> m_pbKeyObject;
    std::vector<byte> m_pbIV;
    void __init()
    {
        NTSTATUS stat = -1;
        DWORD dwData;
        while (stat != 0)
        {
            stat =
                BCryptGetProperty(m_Provider.get(), BCRYPT_OBJECT_LENGTH, (PUCHAR)&dwKeyLen, sizeof(DWORD), &dwData, 0);
            Debug::PrintError(stat);
        }

        m_pbKeyObject.resize(dwKeyLen); // allocate our keylen

        if (BCryptGetProperty(m_Provider.get(), BCRYPT_BLOCK_LENGTH, (PUCHAR)&dwIvLen, sizeof(DWORD), &dwData, 0) >= 0)
            m_pbIV.resize(dwIvLen);

        if (!std::is_same<T, ___detail::RC4Type>::value)
            BCryptSetProperty(m_Provider.get(), BCRYPT_CHAINING_MODE, (PBYTE)BCRYPT_CHAIN_MODE_CBC,
                              sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
    }

    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::AESType>::value, void>::type __SymetricImpl(R)
    {
        BCRYPT_ALG_HANDLE handle;
        if (BCryptOpenAlgorithmProvider(&handle, BCRYPT_AES_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0) >= 0)
        {
            m_Provider.reset(handle);
            __init();
        }
    }
#if _WIN32_WINNT == _WIN32_WINNT_WIN8
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::AES_CMAC_Type>::value, void>::type __SymetricImpl(R)
    {
        BCRYPT_ALG_HANDLE handle;
        if (BCryptOpenAlgorithmProvider(&handle, BCRYPT_AES_CMAC_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0) >= 0)
        {
            m_Provider.reset(handle);
            __init();
        }
    }
#endif
    /*template<>
    void __SymetricImpl(AES_GMAC_Type)
    {
    BCRYPT_ALG_HANDLE handle;
    if(BCryptOpenAlgorithmProvider(&handle,BCRYPT_AES_GMAC_ALGORITHM,MS_PRIMITIVE_PROVIDER,0)
    >= 0) {m_Provider.reset(handle);
    __init();
    }
    }*/

    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::DESType>::value, void>::type __SymetricImpl(R)
    {
        BCRYPT_ALG_HANDLE handle;
        if (BCryptOpenAlgorithmProvider(&handle, BCRYPT_DES_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0) >= 0)
        {
            m_Provider.reset(handle);
            __init();
        }
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::DES3Type>::value, void>::type __SymetricImpl(R)
    {
        BCRYPT_ALG_HANDLE handle;
        if (BCryptOpenAlgorithmProvider(&handle, BCRYPT_3DES_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0) >= 0)
        {
            m_Provider.reset(handle);
            __init();
        }
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::RC2Type>::value, void>::type __SymetricImpl(R)
    {
        BCRYPT_ALG_HANDLE handle;
        if (BCryptOpenAlgorithmProvider(&handle, BCRYPT_RC2_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0) >= 0)
        {
            m_Provider.reset(handle);
            __init();
        }
    }
    template <typename R>
    typename std::enable_if<std::is_same<R, ___detail::RC4Type>::value, void>::type __SymetricImpl(R)
    {
        BCRYPT_ALG_HANDLE handle;
        if (BCryptOpenAlgorithmProvider(&handle, BCRYPT_RC4_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0) >= 0)
        {
            m_Provider.reset(handle);
            __init();
        }
        else
        {
            assert(false);
        }
    }

  public:
    SymmetricImpl(void) : dwDataLen(0), dwKeyLen(0), dwIvLen(0)
    {
        T t;
        __SymetricImpl(t);
    }
    ~SymmetricImpl(void)
    {
        m_Key.reset();
        m_pbKeyObject = ByteVector();
    }

    DWORD GetKeySize()
    {
        return dwKeyLen;
    }
    DWORD GetIVSize()
    {
        return dwIvLen;
    }

    void SetIv(std::vector<byte> IvData)
    {
        if (dwIvLen > 1)
        {
            if (IvData.size() <= dwIvLen)
            {
                std::copy(begin(IvData), end(IvData), &m_pbIV[0]);
                dwIvLen = static_cast<DWORD>(IvData.size());
            }
            else
            {
                m_pbIV.resize(IvData.size());
                std::copy(begin(IvData), end(IvData), &m_pbIV[0]);
                dwIvLen = static_cast<DWORD>(IvData.size());
            }
        }
        else
        {
            m_pbIV.resize(IvData.size());
            std::copy(begin(IvData), end(IvData), &m_pbIV[0]);
            dwIvLen = static_cast<DWORD>(IvData.size());
        }
    }
    void SetKey(std::string Key)
    {
        Hash::CBCryptMD5 hash;
        hash.AddDataToHash(StringToByteVector(Key));
        auto ret = hash.Finalise();
        BCRYPT_KEY_HANDLE handle = nullptr;
        NTSTATUS stat = -1;
        while (stat != 0)
        {
            stat = BCryptGenerateSymmetricKey(m_Provider.get(), &handle, (PUCHAR)m_pbKeyObject.data(),
                                              static_cast<ULONG>(m_pbKeyObject.size()), (PUCHAR)ret.data(),
                                              static_cast<ULONG>(ret.size()), NULL);
            SetIv(ret);
        }

        if (stat == 0)
        {
            m_Key.reset(handle);
        }
    }
    DWORD GetBlockSize()
    {
        DWORD ret, dum;
        BCryptGetProperty(m_Provider.get(), BCRYPT_BLOCK_LENGTH, (PUCHAR)&ret, sizeof(DWORD), &dum, 0);
        return ret;
    }
    std::vector<byte> Encrypt(std::vector<byte> Data)
    {
        std::unique_ptr<byte> pcopyIv(nullptr);
        ULONG paddingmode;
        if (!std::is_same<T, ___detail::RC4Type>::value)
        {
            pcopyIv.reset(new byte[dwIvLen]);

            std::copy(m_pbIV.begin(), m_pbIV.end(),
                      pcopyIv.get()); // copy the iv data. Encrypt will corrupt it.

            paddingmode = BCRYPT_BLOCK_PADDING;
        }
        else
        {
            paddingmode = 0UL;
        }

        DWORD cypherlen = 0;
        BCryptEncrypt(m_Key.get(), (PUCHAR)Data.data(), static_cast<ULONG>(Data.size()), nullptr, (PUCHAR)pcopyIv.get(),
                      pcopyIv.get() ? dwIvLen : 0, nullptr, 0, &cypherlen, paddingmode);
        std::vector<byte> cyperData;
        cyperData.resize(cypherlen);
        DWORD dataLen;
        NTSTATUS stat = -1;
        while (stat != 0)
        {
            stat = BCryptEncrypt(m_Key.get(), (PUCHAR)Data.data(), static_cast<ULONG>(Data.size()), nullptr,
                                 (PUCHAR)pcopyIv.get(), pcopyIv.get() ? dwIvLen : 0, (PUCHAR)cyperData.data(),
                                 static_cast<ULONG>(cyperData.size()), &dataLen, paddingmode);
            Debug::PrintError(stat);
        }
        return cyperData;
    }
    std::vector<byte> Decrypt(std::vector<byte> Data)
    {
        DWORD clearTxtLen;
        std::unique_ptr<byte> pcopyIv(nullptr);
        std::vector<byte> cleartextData;
        ULONG paddingmode = 0;
        if (!std::is_same<T, ___detail::RC4Type>::value)
        {
            pcopyIv.reset(new byte[dwIvLen]);
            std::copy(m_pbIV.begin(), m_pbIV.end(), pcopyIv.get());

            paddingmode = BCRYPT_BLOCK_PADDING;
        }
        else
        {
            paddingmode = 0UL;
        }

        NTSTATUS stat;
        stat = BCryptDecrypt(m_Key.get(), (PUCHAR)Data.data(), static_cast<ULONG>(Data.size()), nullptr,
                             (PUCHAR)pcopyIv.get(), pcopyIv.get() ? dwIvLen : 0, nullptr, 0, &clearTxtLen, paddingmode);
        if (stat)
        {
            Debug::PrintError(stat);
        }
        cleartextData.resize(clearTxtLen);

        stat = BCryptDecrypt(m_Key.get(), (PUCHAR)Data.data(), static_cast<ULONG>(Data.size()), nullptr,
                             (PUCHAR)pcopyIv.get(), pcopyIv.get() ? dwIvLen : 0, (PUCHAR)cleartextData.data(),
                             clearTxtLen, &clearTxtLen, paddingmode);
        if (stat)
        {
            Debug::PrintError(stat);
        }
        m_pbIV.clear();
        return cleartextData;
    }
};

typedef SymmetricImpl<___detail::AESType> CBCryptAES; // Standard: FIPS 197
#if _WIN32_WINNT == _WIN32_WINNT_WIN8
typedef SymmetricImpl<___detail::AES_CMAC_Type> CBCryptAES_CMAC;
#endif
// typedef SymmetricImpl<AES_GMAC_Type> CBCryptAES_GMAC; //Standard: SP800-38D
typedef SymmetricImpl<___detail::DESType> CBCryptDES; // Standard: FIPS 46-3, FIPS 81
typedef SymmetricImpl<___detail::DES3Type> CBCrypt3DES;
typedef SymmetricImpl<___detail::RC2Type> CBCryptRC2; // Standard: RFC 2268
typedef SymmetricImpl<___detail::RC4Type> CBCryptRC4;
}; // namespace Symmetric
}; // namespace Crypto
}; // namespace System

#endif
