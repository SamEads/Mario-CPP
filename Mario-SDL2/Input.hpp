#ifndef INPUT_HPP
#define INPUT_HPP

#include <SDL.h>

class Input
{
	private:
		Uint8 lastKeys[SDL_NUM_SCANCODES];
		Uint8 curKeys[SDL_NUM_SCANCODES];
	public:
		Input();
		bool isPressed(SDL_Scancode scanCode);
		bool wasJustPressed(SDL_Scancode scanCode);
		bool wasJustReleased(SDL_Scancode scanCode);
		void update();
};

#endif // !INPUT_HPP