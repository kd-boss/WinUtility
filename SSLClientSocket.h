#ifndef __ISSICLIENTSOCKET_H
#define __ISSICLIENTSOCKET_H
#include "ClientSocket.h"
#include <type_traits>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <wincrypt.h>
#include <schannel.h>
#include <security.h>

#ifdef UNICODE
#define SEC_TCHAR SEC_WCHAR
#else
#define SEC_TCHAR SEC_CHAR
#endif

class SSPISecurityFunctionTable {
  HMODULE g_hSecurity;
  PSecurityFunctionTable m_ptr;

public:
  SSPISecurityFunctionTable() : g_hSecurity(nullptr), m_ptr(nullptr) {
    OSVERSIONINFO VerInfo;
    std::tstring lpszDLL;

    INIT_SECURITY_INTERFACE pInitSecurityInterface;
    //  Find out which security DLL to use, depending on
    //  whether we are on Win2K, NT or Win9x
    VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!GetVersionEx(&VerInfo))
      throw;

    if (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
        VerInfo.dwMajorVersion == 4) {
      lpszDLL = TEXT("Security.dll"); // NT4_DLL_NAME TEXT("Security.dll")
    } else if (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ||
               VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
      lpszDLL = TEXT("Secur32.dll"); // DLL_NAME TEXT("Secur32.dll")
    } else {
      printf("System not recognized\n");
    }

    //  Load Security DLL
    g_hSecurity = LoadLibrary(lpszDLL.c_str());
    if (g_hSecurity == NULL) {
      printf("Error 0x%lu loading Security Library.\n", GetLastError());
    }

    std::string interfaceName;
    if (std::is_same<std::string, std::tstring>::value)
      interfaceName = "InitSecurityInterfaceA";
    else
      interfaceName = "InitSecurityInterfaceW";

    pInitSecurityInterface = (INIT_SECURITY_INTERFACE)GetProcAddress(
        g_hSecurity, interfaceName.c_str());
    if (pInitSecurityInterface == NULL) {
      printf("Error 0x%lu reading InitSecurityInterface entry point.\n",
             GetLastError());
    }

    m_ptr = pInitSecurityInterface(); // call InitSecurityInterfaceA(void);
    if (m_ptr == NULL) {
      printf("Error 0x%lu reading security interface.\n", GetLastError());
    }
  }
  ~SSPISecurityFunctionTable() {
    FreeLibrary(g_hSecurity);
    g_hSecurity = nullptr;
    m_ptr = nullptr;
  }
  PSecurityFunctionTable operator->() { return m_ptr; }
};

class CertificateStore {
  HCERTSTORE m_store;

public:
  CertificateStore() { m_store = CertOpenSystemStore(0, TEXT("MY")); }
  ~CertificateStore() {
    if (m_store)
      CertCloseStore(&m_store, 0);
  }
  operator HCERTSTORE() { return m_store; }
};

class SslClientSocket : BaseWindow<SslClientSocket, Window, nullptrTraits> {
  
  unsigned long iobuffminsize;
  SSPISecurityFunctionTable m_SSPI;
  SCHANNEL_CRED m_cred;
  CertificateStore m_store;
  CredHandle m_hcred;
  CtxtHandle m_hcontext;
  SOCKET m_socket;
  UINT MSG_SOCKET;
  AddressFamily m_addressFamily;
  SocketType m_socketType;
  ProtocolType m_protocolType;
  ADDRINFOT *m_result;
  std::tstring m_host;
  std::tstring m_port;
  int iport;
  DWORD dwSSPIFlags;
  SECURITY_STATUS status;
  PCCERT_CONTEXT m_RemoteCertContext;
  SecPkgContext_StreamSizes m_streamSizes;
  std::vector<unsigned char> m_iobuffer;
  std::vector<unsigned char> m_sendBuffer;
  int m_iobufferidx;
  bool HanshakeNeeded;
#ifdef UNICODE
  PWSTR getServerName(std::tstring& str) {
	PWSTR value = const_cast<PWSTR>(str.c_str());
    return value;
  }
#else
  PWSTR getServerName(std::tstring str) {
    PWSTR pwszServerName = NULL;
    auto cchServerName =
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    pwszServerName =
        (PWSTR)LocalAlloc(LMEM_FIXED, cchServerName * sizeof(WCHAR));
    if (pwszServerName == NULL) {
      status = SEC_E_INSUFFICIENT_MEMORY;
    }

    cchServerName = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1,
                                        pwszServerName, cchServerName);
    if (cchServerName == 0) {
      status = SEC_E_WRONG_PRINCIPAL;
      if (pwszServerName)
        LocalFree(pwszServerName);
    }
    return pwszServerName;
  }
#endif
  SECURITY_STATUS VerifyServerCertificate() {
    HTTPSPolicyCallbackData polHttps;
    CERT_CHAIN_POLICY_PARA PolicyPara;
    CERT_CHAIN_POLICY_STATUS PolicyStatus;
    CERT_CHAIN_PARA ChainPara;
    PCCERT_CHAIN_CONTEXT pChainContext = NULL;
    // DWORD                                         cchServerName, Status;
    LPSTR rgszUsages[] = {LPSTR(szOID_PKIX_KP_SERVER_AUTH),
                          LPSTR(szOID_SERVER_GATED_CRYPTO),
                          LPSTR(szOID_SGC_NETSCAPE)};

    DWORD cUsages = sizeof(rgszUsages) / sizeof(LPSTR);

    PWSTR pwszServerName = NULL;

    ZeroMemory(&ChainPara, sizeof(ChainPara));
    ChainPara.cbSize = sizeof(ChainPara);
    ChainPara.RequestedUsage.dwType = USAGE_MATCH_TYPE_OR;
    ChainPara.RequestedUsage.Usage.cUsageIdentifier = cUsages;
    ChainPara.RequestedUsage.Usage.rgpszUsageIdentifier = rgszUsages;

    if (!CertGetCertificateChain(NULL, m_RemoteCertContext, NULL,
                                 m_RemoteCertContext->hCertStore, &ChainPara, 0,
                                 NULL, &pChainContext)) {
      if (pChainContext) {
        CertFreeCertificateChain(pChainContext);
      };
      return E_FAIL;
    }

    pwszServerName = getServerName(m_host);
    ZeroMemory(&polHttps, sizeof(HTTPSPolicyCallbackData));
    polHttps.cbStruct = sizeof(HTTPSPolicyCallbackData);
    polHttps.dwAuthType = AUTHTYPE_SERVER;
    polHttps.fdwChecks = 0;
    polHttps.pwszServerName = pwszServerName;

    memset(&PolicyPara, 0, sizeof(PolicyPara));
    PolicyPara.cbSize = sizeof(PolicyPara);
    PolicyPara.pvExtraPolicyPara = &polHttps;

    memset(&PolicyStatus, 0, sizeof(PolicyStatus));
    PolicyStatus.cbSize = sizeof(PolicyStatus);

    if (!CertVerifyCertificateChainPolicy(CERT_CHAIN_POLICY_SSL, pChainContext,
                                          &PolicyPara, &PolicyStatus)) {
      if (pChainContext) {
        CertFreeCertificateChain(pChainContext);
      };
#ifndef UNICODE
      if (pwszServerName) {
        LocalFree(pwszServerName);
      };
#endif

      return E_FAIL;
    }

    if (pChainContext) {
      CertFreeCertificateChain(pChainContext);
    };
#ifdef UNICODE
    if (pwszServerName) {
      LocalFree(pwszServerName);
    };
#endif
    if (PolicyStatus.dwError)
      return SEC_E_UNKNOWN_CREDENTIALS;
    else
      return SEC_E_OK;
  }

  void GetNewClientCredentials() {

    SecPkgContext_IssuerListInfoEx IssuerListInfo;
    PCCERT_CHAIN_CONTEXT pChainContext;
    PCCERT_CONTEXT pCertContext;
    TimeStamp tsExpiry;
    SECURITY_STATUS Status;
    CERT_CHAIN_FIND_BY_ISSUER_PARA FindByIssuerPara;

    Status = m_SSPI->QueryContextAttributes(
        &m_hcontext, SECPKG_ATTR_ISSUER_LIST_EX, (PVOID)&IssuerListInfo);
    if (Status == SEC_E_OK) {
      ZeroMemory(&FindByIssuerPara, sizeof(FindByIssuerPara));
      FindByIssuerPara.cbSize = sizeof(FindByIssuerPara);
      FindByIssuerPara.pszUsageIdentifier = szOID_PKIX_KP_CLIENT_AUTH;
      FindByIssuerPara.dwKeySpec = 0;
      FindByIssuerPara.cIssuer = IssuerListInfo.cIssuers;
      FindByIssuerPara.rgIssuer = IssuerListInfo.aIssuers;

      pChainContext = nullptr;
      while (true) {
        pChainContext = CertFindChainInStore(m_store, X509_ASN_ENCODING, 0,
                                             CERT_CHAIN_FIND_BY_ISSUER,
                                             &FindByIssuerPara, pChainContext);

        if (pChainContext) {
          m_SSPI->FreeCredentialsHandle(&m_hcred);

          ZeroMemory(&m_cred, sizeof(SCHANNEL_CRED));
          m_cred.dwVersion = SCHANNEL_CRED_VERSION;
          m_cred.cCreds = 1;
          m_cred.paCred = &pCertContext;

          Status = m_SSPI->AcquireCredentialsHandle(
              NULL, const_cast<TCHAR *>(UNISP_NAME), SECPKG_CRED_OUTBOUND, NULL,
              &m_cred, NULL, NULL, &m_hcred, &tsExpiry);
        }
      }
    }
  }

  void StartHandshake() {
    SecBufferDesc OutBufferDesc;
    SecBuffer OutBuffer[1];
    DWORD dwSSPIOutFlags;
    TimeStamp tsExpiry;

    OutBuffer[0].pvBuffer = nullptr;
    OutBuffer[0].BufferType = SECBUFFER_TOKEN;
    OutBuffer[0].cbBuffer = 0;

    OutBufferDesc.cBuffers = 1;
    OutBufferDesc.pBuffers = OutBuffer;
    OutBufferDesc.ulVersion = SECBUFFER_VERSION;

    status = m_SSPI->InitializeSecurityContext(
        &m_hcred, NULL, const_cast<SEC_TCHAR *>(m_host.c_str()), dwSSPIFlags, 0,
        SECURITY_NATIVE_DREP, 0, 0, &m_hcontext, &OutBufferDesc,
        &dwSSPIOutFlags, &tsExpiry);

    if (status != SEC_I_CONTINUE_NEEDED) {
      printf("**** Error %lu returned by InitializeSecurityContext (1)\n",
             status);
    }
    if (OutBuffer[0].cbBuffer != 0 && OutBuffer[0].pvBuffer != nullptr) {
      auto sendRet =
          send(m_socket, reinterpret_cast<const char *>(OutBuffer[0].pvBuffer),
               OutBuffer[0].cbBuffer, 0);
      if (sendRet == SOCKET_ERROR || sendRet == 0) {
        m_SSPI->FreeContextBuffer(OutBuffer[0].pvBuffer);
        m_SSPI->DeleteSecurityContext(&m_hcontext);
      }
      m_SSPI->FreeContextBuffer(OutBuffer[0].pvBuffer);
    }
  }

  SECURITY_STATUS HandshakeStep() {
    dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT |
                  ISC_REQ_CONFIDENTIALITY | ISC_RET_EXTENDED_ERROR |
                  ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;
    DWORD dwSSPIOutFlags;
    TimeStamp tsExpiry;
    SECURITY_STATUS mstatus;
    SecBufferDesc OutBufferDesc, InBufferDesc;
    SecBuffer InBuffer[2];
    SecBuffer OutBuffer[1];

    InBuffer[0].pvBuffer = &m_iobuffer[0];
    InBuffer[0].cbBuffer = m_iobufferidx;
    InBuffer[0].BufferType = SECBUFFER_TOKEN;

    InBuffer[1].pvBuffer = nullptr;
    InBuffer[1].cbBuffer = 0;
    InBuffer[1].BufferType = SECBUFFER_EMPTY;

    InBufferDesc.cBuffers = 2;
    InBufferDesc.pBuffers = InBuffer;
    InBufferDesc.ulVersion = SECBUFFER_VERSION;

    OutBuffer[0].pvBuffer = NULL;
    OutBuffer[0].BufferType = SECBUFFER_TOKEN;
    OutBuffer[0].cbBuffer = 0;

    OutBufferDesc.cBuffers = 1;
    OutBufferDesc.pBuffers = OutBuffer;
    OutBufferDesc.ulVersion = SECBUFFER_VERSION;

    mstatus = m_SSPI->InitializeSecurityContext(
        &m_hcred, &m_hcontext, NULL, dwSSPIFlags, 0, SECURITY_NATIVE_DREP,
        &InBufferDesc, 0, NULL, &OutBufferDesc, &dwSSPIOutFlags, &tsExpiry);

    if (mstatus == SEC_E_OK || mstatus == SEC_I_CONTINUE_NEEDED ||
        (FAILED(mstatus) && (dwSSPIOutFlags & ISC_RET_EXTENDED_ERROR))) {
      if (OutBuffer[0].cbBuffer != 0 && OutBuffer[0].pvBuffer != nullptr) {
        int szsend =
            send(m_socket, reinterpret_cast<char *>(OutBuffer[0].pvBuffer),
                 OutBuffer[0].cbBuffer, 0);
        if (szsend == SOCKET_ERROR || szsend == 0) {
          m_SSPI->FreeContextBuffer(OutBuffer[0].pvBuffer);
          m_SSPI->DeleteSecurityContext(&m_hcontext);
          return SEC_E_INTERNAL_ERROR;
        }

        m_SSPI->FreeContextBuffer(OutBuffer[0].pvBuffer);
        printf("%d bytes of handshake data sent\n", szsend);
      }
    }
    if (mstatus == SEC_I_INCOMPLETE_CREDENTIALS)
      return mstatus;

    if (mstatus == SEC_E_INCOMPLETE_MESSAGE) {
      return mstatus;
    }
    if (mstatus == SEC_E_OK) // hanshake sucessful
    {
      printf("Handshake was successful\n");
      if (InBuffer[1].BufferType == SECBUFFER_EXTRA) {

        MoveMemory(&m_iobuffer[0],
                   &m_iobuffer[0] + (m_iobufferidx - InBuffer[0].cbBuffer),
                   InBuffer[0].cbBuffer);
        m_iobufferidx = InBuffer[0].cbBuffer;
      } else
        m_iobufferidx = 0;

      HanshakeNeeded = false;
      mstatus = m_SSPI->QueryContextAttributes(
          &m_hcontext, SECPKG_ATTR_REMOTE_CERT_CONTEXT,
          reinterpret_cast<PVOID>(&m_RemoteCertContext));
      if (mstatus == SEC_E_OK) {
        mstatus = VerifyServerCertificate();
        if (mstatus == SEC_E_OK) {
          if (m_SSPI->QueryContextAttributes(&m_hcontext,
                                             SECPKG_ATTR_STREAM_SIZES,
                                             &m_streamSizes) == SEC_E_OK) {
            iobuffminsize = m_streamSizes.cbHeader + m_streamSizes.cbTrailer +
                            m_streamSizes.cbMaximumMessage;
            m_iobuffer.resize(iobuffminsize);
            m_iobuffer.shrink_to_fit();
            m_sendBuffer.resize(iobuffminsize);
            return mstatus;
          } else
            return mstatus;
        } else
          return mstatus;
      } else {
        return E_FAIL;
      }
    }

    if (InBuffer[1].BufferType == SECBUFFER_EXTRA) {
      MoveMemory(&m_iobuffer[0],
                 &m_iobuffer[0] + (m_iobufferidx - InBuffer[1].cbBuffer),
                 InBuffer[1].cbBuffer);
      m_iobufferidx = InBuffer[1].cbBuffer;

    } else
      m_iobufferidx = 0;
    if (FAILED(mstatus)) {
      printf("**** Error 0x%lu returned by InitializeSecurityContext (2)\n",
             mstatus);
    }
    return mstatus;
  }
  int RecvDecrypted() {

    SecBuffer Buffers[4], *pDataBuffer = nullptr, *pExtraDataBuffer = nullptr;
    SecBufferDesc BuffersDesc;

    Buffers[0].pvBuffer = &m_iobuffer[0];
    Buffers[0].cbBuffer = m_iobufferidx;
    Buffers[0].BufferType = SECBUFFER_DATA;

    Buffers[1].BufferType = SECBUFFER_EMPTY;
    Buffers[2].BufferType = SECBUFFER_EMPTY;
    Buffers[3].BufferType = SECBUFFER_EMPTY;

    BuffersDesc.ulVersion = SECBUFFER_VERSION;
    BuffersDesc.cBuffers = 4;
    BuffersDesc.pBuffers = Buffers;
    SECURITY_STATUS mstatus =
        m_SSPI->DecryptMessage(&m_hcontext, &BuffersDesc, 0, NULL);
    if (mstatus == SEC_E_INCOMPLETE_MESSAGE)
      return 0;
    if (mstatus == SEC_I_CONTEXT_EXPIRED)
      return 0;
    if (mstatus != SEC_E_OK && mstatus != SEC_I_RENEGOTIATE &&
        mstatus != SEC_I_CONTEXT_EXPIRED) {
      printf("**** DecryptMessage failed with %lu \r\n", mstatus);
      return 0;
    }

    for (int i = 0; i < 4; i++) {
      if (pDataBuffer == NULL && Buffers[i].BufferType == SECBUFFER_DATA)
        pDataBuffer = &Buffers[i];
      if (pExtraDataBuffer == NULL && Buffers[i].BufferType == SECBUFFER_EXTRA)
        pExtraDataBuffer = &Buffers[i];
    }

    if (pDataBuffer) {
      if (m_OnRecieve)
        m_OnRecieve(std::vector<unsigned char>(
            reinterpret_cast<unsigned char *>(pDataBuffer->pvBuffer),
            reinterpret_cast<unsigned char *>(pDataBuffer->pvBuffer) +
                pDataBuffer->cbBuffer));
    }
    if (pExtraDataBuffer) {
      MoveMemory(&m_iobuffer[0], pExtraDataBuffer->pvBuffer,
                 pExtraDataBuffer->cbBuffer);
      m_iobufferidx = pExtraDataBuffer->cbBuffer;
    } else
      m_iobufferidx = 0;

    if (mstatus == SEC_I_RENEGOTIATE) {
      this->HanshakeNeeded = true;
      status = HandshakeStep();
    }
    if (pDataBuffer)
      return pDataBuffer->cbBuffer;
    else
      return 0;
  }

  int SendEncrypted(int len) {

    SECURITY_STATUS mstatus;
    SecBufferDesc Messages;
    SecBuffer Buffers[4];

    Buffers[0].pvBuffer = &m_sendBuffer[0];
    Buffers[0].cbBuffer = m_streamSizes.cbHeader;
    Buffers[0].BufferType = SECBUFFER_STREAM_HEADER;

    Buffers[1].pvBuffer = &m_sendBuffer[0] + m_streamSizes.cbHeader;
    Buffers[1].cbBuffer = len;
    Buffers[1].BufferType = SECBUFFER_DATA;

    Buffers[2].pvBuffer = &m_sendBuffer[0] + m_streamSizes.cbHeader + len;
    Buffers[2].cbBuffer = m_streamSizes.cbTrailer;
    Buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;

    Buffers[3].pvBuffer = SECBUFFER_EMPTY;
    Buffers[3].cbBuffer = SECBUFFER_EMPTY;
    Buffers[3].BufferType = SECBUFFER_EMPTY;

    Messages.ulVersion = SECBUFFER_VERSION;
    Messages.cBuffers = 4;
    Messages.pBuffers = Buffers;

    mstatus = m_SSPI->EncryptMessage(&m_hcontext, 0, &Messages, 0);
    if (FAILED(mstatus))
      printf("**** Error 0x%lu returned by EncryptMessage\n", mstatus);

    int totallen =
        Buffers[0].cbBuffer + Buffers[1].cbBuffer + Buffers[2].cbBuffer;
    int szsend = 0;
    while (szsend != totallen) {
      int tszsend =
          send(m_socket, reinterpret_cast<char *>(&m_sendBuffer[0] + szsend),
               totallen - szsend, 0);
      if (tszsend == SOCKET_ERROR || tszsend == 0)
        break;

      szsend += tszsend;
    }
    printf("%d out of %d encrypted bytes sent\r\n", szsend, totallen);
    return szsend;
  }

public:

  SOCKET GetSocket() { return m_socket; }
  PCTSTR ClassName() const { return TEXT("ClientSocket"); }
  std::function<void()> m_OnConnect;
  std::function<void()> m_OnClose;
  std::function<void(std::vector<unsigned char>)> m_OnRecieve;

  SslClientSocket(AddressFamily fam = AddressFamily::IPV4,
                  SocketType type = SocketType::Stream,
                  ProtocolType proto = ProtocolType::TCP)
      : m_hcred{0, 0}, m_hcontext{0, 0}, m_socket{INVALID_SOCKET},
        MSG_SOCKET{::RegisterWindowMessage(TEXT("ClientSocket 1.0"))},
        m_addressFamily{fam}, m_socketType{type}, m_protocolType{proto},
m_result{nullptr}, m_RemoteCertContext{nullptr} {
    dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT |
                  ISC_REQ_CONFIDENTIALITY | ISC_RET_EXTENDED_ERROR |
                  ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;

    status = SEC_I_CONTINUE_NEEDED;
    m_iobufferidx = 0;
    m_iobuffer.resize(65536);
    ZeroMemory(&m_iobuffer[0], sizeof(unsigned char) * 1500);
       m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
      throw WSAException(WSAGetLastError());

    if (!(Create(TEXT("SSLClientSocket"), HWND_MESSAGE))) {
      MessageBox(TEXT("Failed to create ClientSocket backing window"),
                 TEXT("Fatal Error"), MB_OK | MB_ICONEXCLAMATION);
      if (closesocket(m_socket) == SOCKET_ERROR)
        throw WSAException(WSAGetLastError());	
    }

    if (WSAAsyncSelect(m_socket, m_hwnd, MSG_SOCKET,
                       FD_CONNECT | FD_CLOSE | FD_READ) == SOCKET_ERROR) {
      throw WSAException(WSAGetLastError());
    }
  }
  ~SslClientSocket() {
    DestroyWindow();
    if (m_socket != INVALID_SOCKET) {
      //

      if (closesocket(m_socket) == SOCKET_ERROR)
        throw WSAException(WSAGetLastError());
    }
    m_socket = INVALID_SOCKET;
  }

  SECURITY_STATUS Connect(std::tstring host, int port) {
    if (host.empty())
      return SEC_E_CERT_WRONG_USAGE;
    HanshakeNeeded = true;
    iport = port;
    m_host = host;
    m_port = std::to_tstring(port);

    if (m_socket == INVALID_SOCKET) {
      m_socket = socket(static_cast<int>(m_addressFamily),
                        static_cast<int>(m_socketType),
                        static_cast<int>(m_protocolType));
      if (m_socket == INVALID_SOCKET)
        throw WSAException(WSAGetLastError());

      if (WSAAsyncSelect(m_socket, m_hwnd, MSG_SOCKET,
                         FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE) ==
          SOCKET_ERROR) {
        throw WSAException(WSAGetLastError());
      }
    }
    // create an empty client cert. If the server needs a real client cert it
    // will be created during the handshake.
    ZeroMemory(&m_cred, sizeof(m_cred));
    m_cred.dwVersion = SCHANNEL_CRED_VERSION;

    TimeStamp tsExpiry;
    m_cred.grbitEnabledProtocols = 0; // default protocol
    m_cred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS;
    m_cred.dwFlags |= SCH_CRED_MANUAL_CRED_VALIDATION;
    SECURITY_STATUS Status = m_SSPI->AcquireCredentialsHandle(
        NULL, const_cast<TCHAR *>(UNISP_NAME), SECPKG_CRED_OUTBOUND, NULL,
        &m_cred, NULL, NULL, &m_hcred, &tsExpiry);

    if (Status != SEC_E_OK)
      return Status;
    else // empty cert creation successful, we can actually connect now.
    {
      ADDRINFOT hints;
      memset(&hints, 0, sizeof(ADDRINFOT));
      hints.ai_family = static_cast<int>(m_addressFamily);
      hints.ai_protocol = static_cast<int>(m_protocolType);
      hints.ai_socktype = static_cast<int>(m_socketType);

      // get socket inofrmation for hostname or ip address, either one.
      if (GetAddrInfo(host.c_str(), m_port.c_str(), &hints, &m_result) != 0) {
        throw WSAException(WSAGetLastError());
      }

      if (connect(m_socket, m_result->ai_addr,
                  static_cast<int>(m_result->ai_addrlen)) == SOCKET_ERROR) {
        DWORD dret = WSAGetLastError();
        if (dret != WSAEWOULDBLOCK)
          throw WSAException(dret);
      }

      return SEC_E_OK;
    }
  }

  void ReConnect() { Connect(m_host, iport); }
  void Send(std::vector<unsigned char> data) {
    if (data.size() <= m_streamSizes.cbMaximumMessage) {
      std::copy(data.begin(), data.end(),
                m_sendBuffer.begin() + m_streamSizes.cbHeader);
      SendEncrypted(data.size());
    } else {

      while (data.size() > 0) {

        std::copy(data.begin(),
                  (m_streamSizes.cbMaximumMessage > data.size())
                      ? data.end()
                      : data.begin() + m_streamSizes.cbMaximumMessage,
                  m_sendBuffer.begin() + m_streamSizes.cbHeader);
        SendEncrypted(m_streamSizes.cbMaximumMessage);
        data.erase(
            std::remove_if(data.begin(),
                           (m_streamSizes.cbMaximumMessage > data.size())
                               ? data.end()
                               : data.begin() + m_streamSizes.cbMaximumMessage,
                           [](unsigned char) { return true; }),
            data.end());
      }
    }
  }
	DECLARE_WND_CLASS(TEXT("SSLClientSocket"))
	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(MSG_SOCKET,OnSocket)
	END_MSG_MAP()
	
LRESULT OnSocket(UINT msg, WPARAM wParam, LPARAM lParam,BOOL) {
      WORD WSAErr = WSAGETSELECTERROR(lParam);
      WORD WSAevent = WSAGETSELECTEVENT(lParam);
      switch (WSAevent) {
      case FD_CONNECT: {
        if (WSAErr != 0) {
          m_result = m_result->ai_next;
          if (m_result)
            connect(m_socket, m_result->ai_addr,
                    static_cast<int>(m_result->ai_addrlen));
          else
            throw WSAException(DWORD(WSAErr));
        } else {
          // we are connected
          StartHandshake();
        }
        break;
      }
      case FD_READ: {
        if (WSAErr != 0) {
          throw WSAException(DWORD(WSAErr));
        } else {

          int szrecv = recv(
              m_socket, reinterpret_cast<char *>(&m_iobuffer[m_iobufferidx]),
              m_iobuffer.size() - m_iobufferidx, 0);
          m_iobufferidx += szrecv;
          if (HanshakeNeeded == true) {

            printf("%d bytes of handshake data recieved\r\n", szrecv);
            if ((status == SEC_I_CONTINUE_NEEDED ||
                 status == SEC_E_INCOMPLETE_MESSAGE ||
                 status == SEC_I_INCOMPLETE_CREDENTIALS) &&
                szrecv > 0) {

              status = HandshakeStep();

              if (status == SEC_I_INCOMPLETE_CREDENTIALS) {
                printf("getting new client credentials, server asked for "
                       "them\r\n");
                GetNewClientCredentials();
                status = HandshakeStep();
              }
              if (status == SEC_E_OK) {
				  HanshakeNeeded = false;
                if (m_OnConnect)
                  m_OnConnect();
              }
            }

          } else {
            // application data
            int ret = 0;
            do {
              ret = this->RecvDecrypted();
            } while (ret != 0);
          }
          if (szrecv == 0) {

            if (m_OnClose) {
              if (WSAAsyncSelect(m_socket, m_hwnd, 0, 0) == SOCKET_ERROR) {
                throw WSAException(WSAGetLastError());
              }
              if (closesocket(m_socket) == SOCKET_ERROR) {
                throw WSAException(WSAGetLastError());
              }
              m_socket = INVALID_SOCKET;
              m_OnClose();
            }
          } else if (szrecv < 0) {
            DWORD val = WSAGetLastError();

            if (val != WSAEWOULDBLOCK)
              throw WSAException(val);
          }
        }

      } break;
      case FD_WRITE: {

      } break;
      case FD_CLOSE: {
		  if(m_OnClose)
			m_OnClose();
        break;
      }
      }
	  	return 0;
    }

};

#endif
