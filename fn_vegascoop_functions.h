#pragma once
//In here we define a script function
//Script functions must always follow the Cmd_FunctionName_Execute naming convention

DEFINE_COMMAND_PLUGIN(VegasCoop_Init, "Initializes VegasCoop", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_Init_Execute(COMMAND_ARGS)
{
	_MESSAGE("plugintest");

	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsaData;

	int startupValue = WSAStartup(wVersion, &wsaData);

    if (startupValue != 0)
    {
        Console_Print("Could not WSAStartup!");
        return false;
    }

    SOCKET* s = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

    sockaddr_in addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6532);


    if (bind(*s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) 
    {
        Console_Print("Bind failed!");
        closesocket(*s);
        WSACleanup();
        return false;
    }

    Console_Print("Bind successful!");

	*result = 0;	// what the function returns.


	Console_Print("Vegas Coop Startup completed!");

	return true;
}
#endif
