#include "InputManager.hpp"
#include <string.h>
#include <iostream>

Input::Input()
{
	memset(curKeys, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memset(lastKeys, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memset(curMouseButtons, 0, sizeof(int) * 2);
	memset(lastMouseButtons, 0, sizeof(int) * 2);
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

bool Input::isPressed(int mouseButton)
{
	return (curMouseButtons[mouseButton] == 1);
}

bool Input::wasJustPressed(int mouseButton)
{
	return (curMouseButtons[mouseButton] == 1 && lastMouseButtons[mouseButton] == 0);
}

bool Input::wasJustReleased(int mouseButton)
{
	return (curMouseButtons[mouseButton] == 0 && lastMouseButtons[mouseButton] == 1);
}

void Input::update()
{
	memcpy(lastKeys, curKeys, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(curKeys, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}