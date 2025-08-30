#pragma once

#include "GameAPI.h"
#include "TCPSocket.h"

using namespace NECRO;

class VegasCoopBlock
{
private:
	TCPSocket					m_listener;
	std::shared_ptr<TCPSocket>	m_socket;

	// State
	bool m_isConnecting = false;
	bool m_isConnected = false;

public:
	VegasCoopBlock() : m_listener(SocketAddressesFamily::INET)
	{
	}

	void Initialize()
	{
		SocketUtility::Initialize();

		Console_Print("VegasCoopBlock: Initialized!");
	}

	int SetupListener()
	{
		uint16_t inPort = 61532;
		SocketAddress localAddr(AF_INET, INADDR_ANY, inPort);

		int flag = 1;
		m_listener.SetSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
		m_listener.SetSocketOption(SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(int));

		m_listener.Bind(localAddr);
		m_listener.SetBlockingEnabled(false);
		m_listener.Listen();

		Console_Print("VegasCoopBlock: Listening!");

		return 0;
	}

	int AcceptConnection()
	{
		SocketAddress otherAddr;
		std::shared_ptr<TCPSocket> inSock = m_listener.Accept<TCPSocket>(otherAddr);

		if (inSock != nullptr)
		{
			m_socket = inSock;
			return 0;
		}
		else
			return 1;
	}

	int Connect()
	{
		// Make socket
		m_socket = std::make_shared<TCPSocket>(SocketAddressesFamily::INET);

		uint16_t outPort = 61532;
		struct in_addr addr;
		inet_pton(AF_INET, "192.168.1.221", &addr);

		SocketAddress authAddr(AF_INET, addr.s_addr, outPort);

		int flag = 1;
		m_socket->SetSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
		m_socket->SetBlockingEnabled(true); // block on connection
		m_socket->SetRemoteAddressAndPort(authAddr, outPort);

		m_isConnecting = true;

		if (m_socket->Connect(authAddr) != 0)
		{
			m_isConnected = false;
			m_isConnecting = false;
			return 1;
		}
		else
		{
			// Connection succeded!
			m_socket->SetBlockingEnabled(true); // block on connection
			return 0;
		}
	}
};