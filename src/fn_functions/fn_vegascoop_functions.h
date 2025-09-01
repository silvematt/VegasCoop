#pragma once
//In here we define a script function
//Script functions must always follow the Cmd_FunctionName_Execute naming convention

constexpr double RAD_TO_DEG = 180.0f / 3.14159265359;

DEFINE_COMMAND_PLUGIN(VegasCoop_Init, "Initializes VegasCoop", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_Init_Execute(COMMAND_ARGS)
{
	_MESSAGE("plugintest");

	g_vegasCoopBlock.Initialize();

	Console_Print("Vegas Coop Startup completed!");

	*result = 0;	// what the function returns.
	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_SetupListener, "Binds the m_listener and enables listening", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_SetupListener_Execute(COMMAND_ARGS)
{
	_MESSAGE("plugintest");

	if(g_vegasCoopBlock.SetupListener() == 0)
		Console_Print("Listener is set up!");
	else
		Console_Print("There was an issue during SetupListener!");

	*result = 0;	// what the function returns.
	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_AcceptConnection, "Attempts an accept on the listener socket", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_AcceptConnection_Execute(COMMAND_ARGS)
{
	_MESSAGE("plugintest");

	if (g_vegasCoopBlock.AcceptConnection() == 0)
	{
		Console_Print("Connection accepted!");
		*result = 0;	// what the function returns.
	}
	else
	{
		*result = 1;
	}

	return true;
}
#endif

DEFINE_COMMAND_PLUGIN(VegasCoop_Connect, "Connects to a remote peer", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_Connect_Execute(COMMAND_ARGS)
{
	_MESSAGE("plugintest");

	if (g_vegasCoopBlock.Connect() == 0)
	{
		Console_Print("Connection succeeded!");
		*result = 0;	// what the function returns.
	}
	else
	{
		Console_Print("Error during connection!");
		*result = 1;
	}

	return true;
}
#endif



DEFINE_COMMAND_PLUGIN(VegasCoop_NetworkUpdate, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_NetworkUpdate_Execute(COMMAND_ARGS)
{
	g_vegasCoopBlock.NetworkUpdate();

	*result = 0;
	
	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_IsConnectionDone, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_IsConnectionDone_Execute(COMMAND_ARGS)
{
	if (g_vegasCoopBlock.m_isConnected)
		*result = 1;
	else
		*result = 0;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastX, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastX_Execute(COMMAND_ARGS)
{
	const auto& netData = g_vegasCoopBlock.m_socket->GetNetData();

	*result = netData.pos.x;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastY, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastY_Execute(COMMAND_ARGS)
{
	const auto& netData = g_vegasCoopBlock.m_socket->GetNetData();

	*result = netData.pos.y;

	return true;
}
#endif

DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastZ, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastZ_Execute(COMMAND_ARGS)
{
	const auto& netData = g_vegasCoopBlock.m_socket->GetNetData();

	*result = netData.pos.z;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastRotZ, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastRotZ_Execute(COMMAND_ARGS)
{
	const auto& netData = g_vegasCoopBlock.m_socket->GetNetData();

	*result = netData.rotZ * RAD_TO_DEG;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_HasValidNetData, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_HasValidNetData_Execute(COMMAND_ARGS)
{
	const auto& netData = g_vegasCoopBlock.m_socket->GetNetData();

	*result = netData.isValid == false ? 0 : 1;

	return true;
}
#endif



