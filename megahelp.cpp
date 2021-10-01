#include "megahelp.h"

void MegaHelp::DebugConsole()
{
	if (AllocConsole())
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		freopen("CONOUT$", "w", stdout);
#pragma warning(pop)
		std::cout << "\tDEBUG CONSOLE 0.0.1 | by DSRV_Team | " << std::endl;
	}
}
