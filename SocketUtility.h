#pragma once
#ifndef SOCKET_UTILITY_H
#define SOCKET_UTILITY_H

#include "WinSock2.h"

namespace NECRO
{
	namespace SocketUtility
	{
		inline constexpr int SU_NO_ERROR_VAL = 0;

		//---------------------------------------------------------
		// Returns the last error that occurred
		//---------------------------------------------------------
		inline int GetLastError()
		{
			return WSAGetLastError();
		}

		inline bool ErrorIsWouldBlock()
		{
			if (GetLastError() == WSAEWOULDBLOCK)
				return true;

			return false;
		}

		inline bool ErrorIsIsInProgres()
		{
			if (GetLastError() == WSAEINPROGRESS)
				return true;

			return false;
		}

		inline bool ErrorIsIsConnectionRefused()
		{
			if (GetLastError() == WSAECONNREFUSED)
				return true;

			return false;
		}

		//---------------------------------------------------------
		// Initializes Winsock if on windows, otherwise do nothing.
		//---------------------------------------------------------
		inline void Initialize()
		{
#ifdef _WIN32
			WORD wVersion = MAKEWORD(2, 2);
			WSADATA wsaData;

			int startupValue = WSAStartup(wVersion, &wsaData);

			if (startupValue != 0)
			{
				//LOG_ERROR(std::string("Error during SocketUtility::Initialize() [" + std::to_string(startupValue) + "]"));
				return;
			}

			//LOG_OK("SocketUtility::Initialize() successful!");
#endif
		}
	};

}

#endif
