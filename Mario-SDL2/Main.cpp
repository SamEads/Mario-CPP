#include "Game.hpp"
#include "windows.h"
#include "SDL_mixer.h"

int main(int argc, char *argv[])
{
	SetConsoleTitle(TEXT("Super Mario Game"));
	new Game();
	return 0;
}