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
	bool connectionDone = false;

	bool hasBeenUpdatedOnce = false;
	float lastX;
	float lastY;
	float lastZ;
	float lastRot;

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
			connectionDone = true;
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
			m_socket->SetBlockingEnabled(false); // block on connection
			connectionDone = true;
			return 0;
		}
	}

	void QueuePacket(NetworkMessage&& m)
	{
		m_socket->QueuePacket(std::move(m));
	}

	void Send()
	{
		m_socket->Send();
	}

	void Receive()
	{
		int s =  m_socket->Receive();

		while (s >= 16)
		{
			lastX = *reinterpret_cast<float*>(m_socket->m_inBuffer.GetReadPointer());
			m_socket->m_inBuffer.ReadCompleted(4);

			lastY = *reinterpret_cast<float*>(m_socket->m_inBuffer.GetReadPointer());
			m_socket->m_inBuffer.ReadCompleted(4);

			lastZ = *reinterpret_cast<float*>(m_socket->m_inBuffer.GetReadPointer());
			m_socket->m_inBuffer.ReadCompleted(4);

			lastRot = *reinterpret_cast<float*>(m_socket->m_inBuffer.GetReadPointer());
			m_socket->m_inBuffer.ReadCompleted(4);

			hasBeenUpdatedOnce = true;

			s -= 16;
		}
	}
};