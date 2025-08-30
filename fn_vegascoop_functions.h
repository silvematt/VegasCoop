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
