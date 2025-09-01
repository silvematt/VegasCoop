#include "GameSocket.h"

void GameSocket::OnConnectedCallback()
{

}

int GameSocket::ReadCallback()
{
	NetworkMessage& packet = m_inBuffer;

	// Packet parsing like in NECRO would have been a good idea, but again this is just a quick test with unsuitable TCP
	// So let's expect only the pos/rot update packet
	while (packet.GetActiveSize() >= 16)
	{
		m_netData.pos.x = *reinterpret_cast<float*>(packet.GetReadPointer());
		packet.ReadCompleted(4);

		m_netData.pos.y = *reinterpret_cast<float*>(packet.GetReadPointer());
		packet.ReadCompleted(4);

		m_netData.pos.z = *reinterpret_cast<float*>(packet.GetReadPointer());
		packet.ReadCompleted(4);

		m_netData.rotZ = *reinterpret_cast<float*>(packet.GetReadPointer());
		packet.ReadCompleted(4);

		m_netData.isValid = true;
	}

	return 0;
}
