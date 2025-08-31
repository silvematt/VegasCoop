#pragma once
//In here we define a script function
//Script functions must always follow the Cmd_FunctionName_Execute naming convention

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



DEFINE_COMMAND_PLUGIN(VegasCoop_SendPosition, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_SendPosition_Execute(COMMAND_ARGS)
{
	TESObjectREFR* player = (*g_thePlayer);
	float x = player->posX;
	float y = player->posY;
	float z = player->posZ;
	float ang = player->rotZ;

	Packet p;
	
	p << x;
	p << y;
	p << z;
	p << ang;

	NetworkMessage m(p);
	g_vegasCoopBlock.QueuePacket(std::move(m));
	g_vegasCoopBlock.Send();
	*result = 0;
	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_RecvAndSet, "Sends the current's player position to the other end", false, kParams_OneObjectRef)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_RecvAndSet_Execute(COMMAND_ARGS)
{
	TESObjectREFR* targetRef = nullptr;

	// Extract the reference argument
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &targetRef))
	{
		g_vegasCoopBlock.Receive();

		NiVector3 pos;
		pos.x = g_vegasCoopBlock.lastX;
		pos.y = g_vegasCoopBlock.lastY;
		pos.z = g_vegasCoopBlock.lastZ;

		targetRef->rotZ = g_vegasCoopBlock.lastRot;

		*result = 0;
		return true;
	}
	else
	{
		Console_Print("NO TARGET REF!");
		_MESSAGE("NO TARGET REF");
	}

	
	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_IsConnectionDone, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_IsConnectionDone_Execute(COMMAND_ARGS)
{
	if (g_vegasCoopBlock.connectionDone)
		*result = 0;
	else
		*result = 1;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastX, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastX_Execute(COMMAND_ARGS)
{
	*result = g_vegasCoopBlock.lastX;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastY, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastY_Execute(COMMAND_ARGS)
{
	*result = g_vegasCoopBlock.lastY;

	return true;
}
#endif

DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastZ, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastZ_Execute(COMMAND_ARGS)
{
	*result = g_vegasCoopBlock.lastZ;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_GetLastRotZ, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_GetLastRotZ_Execute(COMMAND_ARGS)
{
	*result = g_vegasCoopBlock.lastRot;

	return true;
}
#endif


DEFINE_COMMAND_PLUGIN(VegasCoop_HasBeenUpdatedOnce, "Sends the current's player position to the other end", false, NULL)

#if RUNTIME	// unnecessary if we only compile in non-GECK mode.
// Otherwise, GECK plugins do not need to know a function's code.
bool Cmd_VegasCoop_HasBeenUpdatedOnce_Execute(COMMAND_ARGS)
{
	*result = g_vegasCoopBlock.hasBeenUpdatedOnce == false ? 1 : 0;

	return true;
}
#endif



