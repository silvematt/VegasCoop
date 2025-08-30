#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include "WinSock2.h"

#include <cstdint>
#include <string>

namespace NECRO
{
	//--------------------------------------------------------------
	// Wraps a sockaddr in a class and provides useful constructors.
	//--------------------------------------------------------------
	class SocketAddress
	{
	private:
		friend class TCPSocket;
		sockaddr m_addr;

	public:
		const size_t GetSize() const;
		SocketAddress() = default;

		SocketAddress(uint16_t family, uint32_t inAddress, uint16_t inPort)
		{
			sockaddr_in* ptr = reinterpret_cast<sockaddr_in*>(&m_addr);

			memset(ptr, 0, sizeof(*ptr));

			ptr->sin_family = family;
			ptr->sin_addr.S_un.S_addr = inAddress;
			ptr->sin_port = htons(inPort);
		}

		SocketAddress(uint16_t family, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint16_t inPort)
		{
			sockaddr_in* ptr = reinterpret_cast<sockaddr_in*>(&m_addr);

			memset(ptr, 0, sizeof(*ptr));

			ptr->sin_family = family;
			ptr->sin_addr.S_un.S_un_b.s_b1 = b1;
			ptr->sin_addr.S_un.S_un_b.s_b2 = b2;
			ptr->sin_addr.S_un.S_un_b.s_b3 = b3;
			ptr->sin_addr.S_un.S_un_b.s_b4 = b4;
			ptr->sin_port = htons(inPort);
		}

		SocketAddress(const sockaddr& inSockAddr)
		{
			memcpy(&m_addr, &inSockAddr, sizeof(inSockAddr));
		}

		std::string RemoteAddressAndPortToString() const
		{
			const sockaddr_in* addr_in = reinterpret_cast<const sockaddr_in*>(&m_addr);
			const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&addr_in->sin_addr.S_un.S_addr);

			std::string result = std::to_string(static_cast<int>(bytes[0])) + "." +
				std::to_string(static_cast<int>(bytes[1])) + "." +
				std::to_string(static_cast<int>(bytes[2])) + "." +
				std::to_string(static_cast<int>(bytes[3])) + ":" +
				std::to_string(ntohs(addr_in->sin_port));

			return result;
		}

		std::string RemoteAddressToString() const
		{
			const sockaddr_in* addr_in = reinterpret_cast<const sockaddr_in*>(&m_addr);
			const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&addr_in->sin_addr.S_un.S_addr);

			std::string result = std::to_string(static_cast<int>(bytes[0])) + "." +
				std::to_string(static_cast<int>(bytes[1])) + "." +
				std::to_string(static_cast<int>(bytes[2])) + "." +
				std::to_string(static_cast<int>(bytes[3]));

			return result;
		}
	};

	inline const size_t SocketAddress::GetSize() const
	{
		return sizeof(m_addr);
	}

}

#endif
