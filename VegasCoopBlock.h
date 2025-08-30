#pragma once

#include "GameAPI.h"
#include "TCPSocket.h"

using namespace NECRO;

class VegasCoopBlock
{
private:
	TCPSocket m_listener;
	TCPSocket m_socket;

public:
	VegasCoopBlock() : m_listener(SocketAddressesFamily::INET), m_socket(SocketAddressesFamily::INET)
	{
	}

	void Initialize()
	{
		SocketUtility::Initialize();

		Console_Print("VegasCoopBlock: Initialized!");
	}

	void SetupListener()
	{
		uint16_t inPort = 61532;
		SocketAddress localAddr(AF_INET, INADDR_ANY, inPort);
		int flag = 1;

		m_listener.SetSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
		m_listener.SetSocketOption(SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(int));

		m_listener.Bind(localAddr);
		m_listener.SetBlockingEnabled(false);
		m_listener.Listen();
	}

	void Accept()
	{
		SocketAddress otherAddr;
		std::shared_ptr<TCPSocket> inSock = m_listener.Accept<TCPSocket>(otherAddr);
	}
};