#pragma once

#include "GameAPI.h"
#include "GameSocket.h"

using namespace NECRO;


class VegasCoopBlock
{
private:
	TCPSocket					m_listener;

public:
	std::shared_ptr<GameSocket>	m_socket;

	bool						m_isConnected;		// true on theserver-side after the Accept() completes, true on the client-side after Connect() succeedes
													// when this is true, m_socket is writable/readable, but the other side may not be ready.
													// should send greet packet to have a isReady bool

public:
	VegasCoopBlock() : m_listener(SocketAddressesFamily::INET), m_isConnected(false)
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
		std::shared_ptr<GameSocket> inSock = m_listener.Accept<GameSocket>(otherAddr);

		if (inSock != nullptr)
		{
			m_socket = inSock;
			m_isConnected = true;

			Console_Print("VegasCoopBlock: Accepted!");
			return 0;
		}
		else
			return 1;
	}

	int Connect()
	{
		// Make socket
		m_socket = std::make_shared<GameSocket>(SocketAddressesFamily::INET);
		
		uint16_t outPort = 61532;
		struct in_addr addr;
		inet_pton(AF_INET, "192.168.1.221", &addr);

		SocketAddress authAddr(AF_INET, addr.s_addr, outPort);

		int flag = 1;

		m_socket->SetSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
		m_socket->SetBlockingEnabled(true); // block on connection
		m_socket->SetRemoteAddressAndPort(authAddr, outPort);

		if (m_socket->Connect(authAddr) != 0)
		{
			_MESSAGE("Error occurred during VegasCoopBlock::Connect.");
			return 1;
		}
		else
		{
			// Connection succeded!
			m_socket->SetBlockingEnabled(false); // block on connection
			m_isConnected = true;

			m_socket->OnConnectedCallback();

			return 0;
		}
	}

	void NetworkUpdate()
	{
		if (m_isConnected)
		{
			// Receive
			int recv = m_socket->Receive();

			// Apply [currently done via OBScript]

			// Send
			TESObjectREFR* player = (*g_thePlayer);

			Packet p;

			p << player->posX;
			p << player->posY;
			p << player->posZ;
			p << player->rotZ;

			NetworkMessage m(p);
			m_socket->QueuePacket(std::move(m));
			int sent = m_socket->Send();
		}
	}
};