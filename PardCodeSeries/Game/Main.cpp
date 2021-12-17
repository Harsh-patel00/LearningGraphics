#include <iostream>

#include "GEngine/Game/GGame.h"

int main()
{
	try
	{
		GGame game;
		game.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}

	return 0;
}