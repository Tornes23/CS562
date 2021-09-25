#include <iostream>
#include "sdl2/SDL.h"
#include "Utils.h"
#include "Window.h"

#undef main
int main(char* argv[], int argc)
{
	//handle the console arguments if necesary

	//InputManager.Initialize();
	//RenderManager.Initialize();
	Utils::InitSDL();
	Window.Create();
	Utils::InitGL();
	//Editor.Initialize();
	//GOManager.LoadLevel("");

	while (!Window.IsClosed())
	{
		//Editor.StartFrame();
		//InputManager.StartFrame();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Window.CloseWindow();
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				//InputManager.HandleEvent(event);
				break;
			}
		}
	}

	Window.ShutDown();
	SDL_Quit();

	std::cerr << "Hello World\n";
	return 0;
}