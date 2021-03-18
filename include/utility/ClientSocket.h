#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <Winsock2.h>
#include <ws2tcpip.h>
#include "BaseWindow.h"
#include <functional>
#include <vector>
#include <memory>



class WSAException {
PTSTR buffer;
public:
  WSAException(DWORD val) : m_val(val) {}
  DWORD m_val;
  
  PTSTR what() {
    
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                  HINST_THISCOMPONENT, m_val, 0, buffer, 2, nullptr);
    
  }
  ~WSAException()
  {
	  ::LocalFree(buffer);
  }
};

class WSAInitHandler {
  WSADATA data;

public:
  WSAInitHandler(int verMajor = 2, int verMinor = 2) {
    int ret;
    if ((ret = WSAStartup(MAKEWORD(verMajor, verMinor), &data)) != 0) {
      // version number too high, recoverable error, retry using the highest
      // available.
      if (ret == WSAVERNOTSUPPORTED) {
        if ((ret = WSAStartup(data.wHighVersion, &data)) != 0)
          throw WSAException(ret); // Something is sevierly worng, throw.
      } else // non recoverable error, throw.
      {
        throw WSAException(ret);
      }
    }
  }
  ~WSAInitHandler() {
    if (WSACleanup() != 0) {
      throw WSAException(WSAGetLastError());
    }
  }
};

enum class AddressFamily : int {
  Unspecified = 0,
  IPV4 = 2,
  Ipx = 6,
  AppleTalk = 16,
  NetBios = 17,
  IPV6 = 23,
  IRDA = 26,
  Bluetooth = 32
};

enum class SocketType : int {
  Stream = 1,
  Datagram = 2,
  Raw = 3,
  ReliableDatagram = 4,
  SequentialPacket = 5
};

enum class ProtocolType : int {
  ICMP = 1,
  IGMP = 2,
  BluetoothRfCom = 3,
  TCP = 6,
  UDP = 17,
  ICMPV6 = 58,
  ReliableMulticast = 113
};

/*
Event: FD_CONNECT
Event: FD_CLOSE
Event: FD_ACCEPT
Event: FD_ADDRESS_LIST_CHANGE
Event: FD_GROUP_QOS
Event: FD_OOB
Event: FD_QOS
Event: FD_READ
Event: FD_WRITE
Event: FD_ROUTING_INTERFACE_CHANGE
 */

class ClientSocket : public BaseWindow<ClientSocket, Window, nullptrTraits> {
  SOCKET m_socket;
  UINT MSG_SOCKET;
  AddressFamily m_addressFamily;
  SocketType m_socketType;
  ProtocolType m_protocolType;
  ADDRINFOT *m_result;
  std::tstring m_host;
  std::tstring m_port;
  int iport;
  std::vector<unsigned char> m_buffer;
public:
 
  SOCKET GetSocket() { return m_socket; }
  PCTSTR ClassName() const { return TEXT("ClientSocket"); }
  std::function<void()> m_OnConnect;
  std::function<void()> m_OnClose;
  std::function<void(std::vector<unsigned char>)> m_OnRecieve;

  ClientSocket(AddressFamily fam, SocketType type, ProtocolType proto)
      : m_socket(INVALID_SOCKET),
        MSG_SOCKET(::RegisterWindowMessage(TEXT("ClientSocket 1.0"))),
        m_addressFamily(fam), m_socketType(type), m_protocolType(proto),
        m_result(nullptr) {

    m_buffer.resize(1500);
    memset(m_buffer.data(), 0, sizeof(unsigned char) * m_buffer.size());
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
      throw WSAException(WSAGetLastError());

    if (!(Create(TEXT("ClientSocket"), HWND_MESSAGE))) {
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
  ~ClientSocket() {
    DestroyWindow();
    if (m_socket != INVALID_SOCKET) {

      if (closesocket(m_socket) == SOCKET_ERROR)
        throw WSAException(WSAGetLastError());
    }
    m_socket = INVALID_SOCKET;
  }

  void Connect(std::tstring host, int port) {
    iport = port;
    m_host = host;
    m_port = std::to_tstring(iport);
    if (m_socket == INVALID_SOCKET) {
      m_socket = socket(static_cast<int>(m_addressFamily),
                        static_cast<int>(m_socketType),
                        static_cast<int>(m_protocolType));
      if (m_socket == INVALID_SOCKET)
        throw WSAException(WSAGetLastError());

      if (!WSAAsyncSelect(m_socket, m_hwnd, MSG_SOCKET,
                          FD_CONNECT | FD_CLOSE | FD_READ)) {
        throw WSAException(WSAGetLastError());
      }
    }

    ADDRINFOT hints;
    memset(&hints, 0, sizeof(ADDRINFOT));
    hints.ai_family = static_cast<int>(m_addressFamily);
    hints.ai_protocol = static_cast<int>(m_protocolType);
    hints.ai_socktype = static_cast<int>(m_socketType);

    if (GetAddrInfo(host.c_str(), m_port.c_str(), &hints, &m_result) != 0) {
      throw WSAException(WSAGetLastError());
    }
    connect(m_socket, m_result->ai_addr,
            static_cast<int>(m_result->ai_addrlen));
  }
  void ReConnect() { Connect(m_host, iport); }
  void Send(std::vector<unsigned char> &data) {
    int ret = 0;
    unsigned int total = 0;
    do {
      ret = send(m_socket, reinterpret_cast<char *>(data.data() + total),
                 data.size() - total, 0);

      if (ret != SOCKET_ERROR) {
        total += ret;
      } else {
        throw WSAException(WSAGetLastError());
      }
    } while (total != data.size());
  }


    LRESULT OnSocket(UINT uMsg, WPARAM wParam, LPARAM lParam , BOOL) {
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
          if (m_OnConnect)
            m_OnConnect();
        }
        break;
      }
      case FD_READ: {
        if (WSAErr != 0) {
          throw WSAException(DWORD(WSAErr));
        }
        int szrecv = recv(m_socket, reinterpret_cast<char *>(&m_buffer[0]),
                          m_buffer.size(), 0);
        if (szrecv > 0 && szrecv != SOCKET_ERROR) {
          if (m_OnRecieve)
            m_OnRecieve(std::vector<unsigned char>(m_buffer.begin(),
                                                   m_buffer.begin() + szrecv));
        } else {
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
          } else {
            DWORD val = WSAGetLastError();

            if (val != WSAEWOULDBLOCK)
              throw WSAException(val);
          }
        }
        break;
      }
      case FD_CLOSE: {
        //                        if(m_socket != INVALID_SOCKET)
        //                        {
        //                            if(WSAAsyncSelect(m_socket,m_hwnd,0,0) ==
        //                            SOCKET_ERROR)
        //                            {
        //                                throw WSAException(WSAGetLastError());
        //                            }
        //                            if(closesocket(m_socket) == SOCKET_ERROR)
        //                            {
        //                                throw WSAException(WSAGetLastError());
        //                            }
        //                            m_socket = INVALID_SOCKET;
        //                        }
        //                        if(m_OnClose)
        //                            m_OnClose();

        break;
      }
      }
	  return 0;
    }
	
	
	DECLARE_WND_CLASS(TEXT("ClientSocket"))
	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(MSG_SOCKET,OnSocket)
	END_MSG_MAP()

};

#endif // CLIENTSOCKET_H
