#pragma once

#include "GameAPI.h"

/*
* TCP really doesn't make sense for a mod like this
* This is just a quick test done in TCP because I already had a TCP network library built (NECRO)
*/

#include "TCPSocket.h"
#include <NiPoint.h>

using namespace NECRO;

struct NetData
{
	bool		isValid = false;
	NiPoint3	pos;
	float		rotZ;
};

class GameSocket : public TCPSocket
{
private:
	NetData	m_netData;

public:
	GameSocket(SocketAddressesFamily family) : TCPSocket(family)
	{
	}

	GameSocket(sock_t sock) : TCPSocket(sock)
	{
	}

	const NetData& GetNetData() const
	{
		return m_netData;
	}

	void	OnConnectedCallback() override;
	int		ReadCallback() override;
};
