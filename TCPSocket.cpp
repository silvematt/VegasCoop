#include "TCPSocket.h"
#include "SocketUtility.h"

#include <iostream>
#include <string>


namespace NECRO
{
	TCPSocket::TCPSocket(SocketAddressesFamily family)
	{
		m_ShutDown = false;

		m_socket = socket(static_cast<int>(family), SOCK_STREAM, IPPROTO_TCP);

		if (m_socket == INVALID_SOCKET)
		{
			//LOG_ERROR(std::string("Error 1 during TCPSocket::Create()"));
			//LOG_ERROR(std::to_string(SocketUtility::GetLastError()));
		}
	}

	TCPSocket::TCPSocket(sock_t inSocket)
	{
		m_ShutDown = false;

		m_socket = inSocket;

		if (m_socket == INVALID_SOCKET)
		{
			//LOG_ERROR(std::string("Error 2 during TCPSocket::Create()"));
			//LOG_ERROR(std::to_string(SocketUtility::GetLastError()));
		}
	}


	bool TCPSocket::IsShutDown()
	{
		return m_ShutDown;
	}

	bool TCPSocket::IsOpen()
	{
		return !m_Closed;
	}

	int TCPSocket::Bind(const SocketAddress& addr)
	{
		int err = bind(m_socket, &addr.m_addr, addr.GetSize());

		if (err != 0)
		{
			//LOG_ERROR(std::string("Error during TCPSocket::Bind() [" + std::to_string(SocketUtility::GetLastError()) + "]"));
			return SocketUtility::GetLastError();
		}

		return SocketUtility::SU_NO_ERROR_VAL;
	}

	int TCPSocket::Listen(int backlog)
	{
		int err = listen(m_socket, backlog);

		if (err != 0)
		{
			//LOG_ERROR(std::string("Error during TCPSocket::Listen() [" + std::to_string(SocketUtility::GetLastError()) + "]"));
			return SocketUtility::GetLastError();
		}

		return SocketUtility::SU_NO_ERROR_VAL;
	}

	int TCPSocket::Connect(const SocketAddress& addr)
	{
		int err = connect(m_socket, &addr.m_addr, addr.GetSize());

		if (err != 0)
		{
			if (!SocketUtility::ErrorIsWouldBlock() && !SocketUtility::ErrorIsIsInProgres())
			{
				//LOG_ERROR(std::string("Error during TCPSocket::Connect() [") + std::to_string(SocketUtility::GetLastError()) + "]");
				return SocketUtility::GetLastError();
			}
		}

		return SocketUtility::SU_NO_ERROR_VAL;
	}

	void TCPSocket::QueuePacket(NetworkMessage&& pckt)
	{
		m_outQueue.push(std::move(pckt));

		// Update pfd events
		m_pfd.events = POLLIN | (HasPendingData() ? POLLOUT : 0);
	}

	int TCPSocket::Send()
	{
		if (m_outQueue.empty())
			return 0;

		NetworkMessage& out = m_outQueue.front();

		int bytesSent = 0;
		bytesSent = send(m_socket, reinterpret_cast<const char*>(out.GetReadPointer()), out.GetActiveSize(), 0);

		if (bytesSent < 0)
		{
			if (SocketUtility::ErrorIsWouldBlock())
				return 0;

			// LOG_ERROR(std::string("Error during TCPSocket::Send() [") + std::to_string(SocketUtility::GetLastError()) + "]");
			return -1;
		}

		// Mark that 'bytesSent' were sent
		out.ReadCompleted(bytesSent);

		if (out.GetActiveSize() == 0)
			m_outQueue.pop(); // if whole packet was sent, pop it from the queue, otherwise we had a short send and will come back later

		SendCallback();

		// Update pfd events
		m_pfd.events = POLLIN | (HasPendingData() ? POLLOUT : 0);

		return bytesSent;
	}

	int TCPSocket::SysSend(const char* buf, int len)
	{
		int bytesSent = 0;
		bytesSent = send(m_socket, buf, len, 0);

		if (bytesSent < 0)
		{
			if (SocketUtility::ErrorIsWouldBlock())
				return 0;

			//Shutdown();
			Close();

			//LOG_ERROR(std::string("Error during TCPSocket::SysSend() [") + std::to_string(SocketUtility::GetLastError()) + "]");
			return -1;
		}

		return bytesSent;
	}

	int TCPSocket::Receive()
	{
		if (!IsOpen())
			return 0;

		m_inBuffer.CompactData();
		m_inBuffer.EnlargeBufferIfNeeded();

		// Manually write on the inBuffer
		int bytesReceived = 0;
		size_t sslBytesReceived = 0;
			bytesReceived = recv(m_socket, reinterpret_cast<char*>(m_inBuffer.GetWritePointer()), m_inBuffer.GetRemainingSpace(), 0);

			if (bytesReceived < 0)
			{
				if (SocketUtility::ErrorIsWouldBlock())
					return 0;

				//LOG_ERROR(std::string("Error during TCPSocket::Receive() [") + std::to_string(SocketUtility::GetLastError()) + "]");
				return -1;
			}

		// Make sure to update the write pos
		m_inBuffer.WriteCompleted(bytesReceived);

		//LOG_INFO("Received {} bytes of something!", bytesReceived);

		if (ReadCallback() == -1)	// this will handle the data we've received, unless it returns -1 (error)
			return -1;

		return bytesReceived;
	}

	int TCPSocket::SysReceive(char* buf, int len)
	{
		int bytesReceived = 0;
		bytesReceived = recv(m_socket, buf, len, 0);

		if (bytesReceived < 0)
		{
			if (SocketUtility::ErrorIsWouldBlock())
				return 0;

			//Shutdown();
			Close();

			// LOG_ERROR(std::string("Error during TCPSocket::Receive() [") + std::to_string(SocketUtility::GetLastError()) + "]");
			return -1;
		}

		return bytesReceived;
	}

	int TCPSocket::SetBlockingEnabled(bool blocking)
	{
		u_long mode = blocking ? 0 : 1;
		int result = ioctlsocket(m_socket, FIONBIO, &mode);

		if (result != 0)
		{
			// LOG_ERROR(std::string("Error during TCPSocket::SetBlockingEnabled(" + std::to_string(blocking) + ") [") + std::to_string(SocketUtility::GetLastError()) + "]");
			return SocketUtility::GetLastError();
		}

		return SocketUtility::SU_NO_ERROR_VAL;
	}

	int TCPSocket::SetSocketOption(int lvl, int optName, const char* optVal, int optLen)
	{
		int optResult = setsockopt(m_socket, lvl, optName, optVal, optLen);

		if (optResult != 0)
		{
			// LOG_ERROR(std::string("Error during TCPSocket::SetSocketOption(" + std::to_string(optName) + ") [") + std::to_string(SocketUtility::GetLastError()) + "]");
			return SocketUtility::GetLastError();
		}

		return SocketUtility::SU_NO_ERROR_VAL;
	}

	int TCPSocket::Shutdown()
	{
		if (m_ShutDown)
			return 0;

		m_ShutDown = true;

		int result = shutdown(m_socket, SD_SEND);

		/*
		if (result < 0)
			LOG_ERROR("Error while shutting down the socket");
		*/
		return result;
	}

	int TCPSocket::Close()
	{
		if (m_Closed)
			return 0;

		m_Closed = true;

		int result = 0;

		result = closesocket(m_socket);

		return result;
	}
}
