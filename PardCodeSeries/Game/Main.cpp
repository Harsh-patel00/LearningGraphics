#include "GEngine/Game/GGame.h"
#include "GEngine/Console/GConsole.h"

int main()
{
	try
	{
		GGame game;
		game.Run();
	}
	catch (const std::exception& e)
	{
		GConsole::LOGERROR(e.what());
		return 1;
	}

	return 0;
}