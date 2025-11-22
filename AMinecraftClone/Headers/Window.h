#pragma once
#include <SDL.h>
class Window
{
public:
	SDL_Window* m_Window;
	SDL_GLContext m_Context;

	void Init(); //initializes SDL and initializes an SDL window
	void Termintate(); //terminates the SDL window when the game closes and quits SDL
};

