#include "Input.hpp"
#include <string.h>

Input::Input()
{
	memset(lastKeys, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(curKeys, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

bool Input::isPressed(SDL_Scancode scanCode)
{
	return curKeys[scanCode];
}

bool Input::wasJustPressed(SDL_Scancode scanCode)
{
	return curKeys[scanCode] && !lastKeys[scanCode];
}

bool Input::wasJustReleased(SDL_Scancode scanCode)
{
	return !curKeys[scanCode] && lastKeys[scanCode];
}

void Input::update()
{
	// Copy current keys to last keys
	memcpy(lastKeys, curKeys, sizeof(Uint8) * SDL_NUM_SCANCODES);
	// Copy keyboard state to current keys
	memcpy(curKeys, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}