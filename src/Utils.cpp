#include <iostream>
#include "sdl2/SDL.h"
#include "gl/glew.h"
#include "Utils.h"
#include "Window.h"

void Utils::InitSDL()
{
	//initializing the SDL library to video only
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Could not initialize SDL: " << SDL_GetError() << "\n";
		exit(1);
	}
}

void Utils::InitGL()
{
	//initializing the glew library
	if (glewInit() != GLEW_OK)
	{
		SDL_GL_DeleteContext(Window.GetSDLContext());
		SDL_DestroyWindow(Window.GetSDLWindow());
		std::cerr << "GLEW Error: Failed to init" << "\n";
		SDL_Quit();
		exit(1);
	}

	//depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//back face removal
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}
