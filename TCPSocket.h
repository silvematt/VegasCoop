#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#ifdef _WIN32
#include "WinSock2.h"
#include <WS2tcpip.h>
#endif

#include <memory>
#include <cstdint>
#include <queue>

#include "SocketAddress.h"
#include "NetworkMessage.h"

#include "SocketUtility.h"

namespace NECRO
{
	typedef SOCKET sock_t;

	inline constexpr int READ_BLOCK_SIZE = 4096;

	inline constexpr int TCP_LISTEN_DEFUALT_BACKLOG = SOMAXCONN;

	enum class SocketAddressesFamily
	{
		INET = AF_INET,
		INET6 = AF_INET6
	};

	//-------------------------------------------------------
	// Defines a TCP Socket object.
	//-------------------------------------------------------
	class TCPSocket
	{
	protected:
		friend class SocketAddress;
		sock_t m_socket;

		SocketAddress	m_remoteAddress;
		uint16_t		m_remotePort;

		// Read/Write buffers
		NetworkMessage				m_inBuffer;
		std::queue<NetworkMessage>	m_outQueue;

		bool m_ShutDown = false;
		bool m_Closed = false;

	public:
		// If the outqueue is empty, only poll for POLLIN events, otherwise, also POLLOUT events
		// This is better than the callback Send() approach because even if a Send fails because the socket was not writable at the time of the callback, we'll still try to send the packets later
		pollfd m_pfd;

		TCPSocket(SocketAddressesFamily family);
		TCPSocket(sock_t inSocket);

		virtual void	OnConnectedCallback() {};
		virtual int		ReadCallback() { return 0; };
		virtual void	SendCallback() {};

		bool						IsOpen();
		bool						IsShutDown();

		int							Bind(const SocketAddress& addr);
		int							Listen(int backlog = TCP_LISTEN_DEFUALT_BACKLOG);

		// Templated Accept
		template<typename T = TCPSocket>
		std::shared_ptr<T> Accept(SocketAddress& addr)
		{
			static_assert(std::is_base_of<TCPSocket, T>::value || std::is_same<TCPSocket, T>::value, "T must be TCPSocket or derived from it");

			int addrLen = addr.GetSize();
			sock_t inSocket = accept(m_socket, &addr.m_addr, &addrLen);

			if (inSocket != INVALID_SOCKET)
			{
				std::shared_ptr<T> newSocket = std::make_shared<T>(inSocket);
				newSocket->m_remoteAddress = addr;
				newSocket->m_remotePort = ntohs(reinterpret_cast<sockaddr_in*>(&addr.m_addr)->sin_port);
				return newSocket;
			}
			else
			{
				if (!SocketUtility::ErrorIsWouldBlock())
				{
					//LOG_ERROR(std::string("Error during TCPSocket::Accept()"));
				}
				return nullptr;
			}
		}

		sock_t AcceptSys()
		{
			sockaddr_in otherAddress;
			int otherAddressLength = sizeof(otherAddress);
			sock_t acceptedSocket = accept(m_socket, (struct sockaddr*)&otherAddress, &otherAddressLength);
			return acceptedSocket;
		}

		int							Connect(const SocketAddress& addr);

		void						QueuePacket(NetworkMessage&& pckt);
		int							Send();
		int							SysSend(const char* buf, int len);

		int							Receive();
		int							SysReceive(char* buf, int len);


		std::string GetRemoteAddressAndPort()
		{
			return m_remoteAddress.RemoteAddressAndPortToString();
		}

		std::string GetRemoteAddress()
		{
			return m_remoteAddress.RemoteAddressToString();
		}

		void SetRemoteAddressAndPort(const SocketAddress& s, const uint16_t& p)
		{
			m_remoteAddress = s;
			m_remotePort = p;
		}

		uint16_t GetPort()
		{
			return m_remotePort;
		}

		bool HasPendingData() const
		{
			return !m_outQueue.empty();
		}

		int							Shutdown();
		int							Close();

		sock_t						GetSocketFD() { return m_socket; };

		int							SetBlockingEnabled(bool blocking);
		int							SetSocketOption(int lvl, int optName, const char* optVal, int optLen);

	};

}
#endif
