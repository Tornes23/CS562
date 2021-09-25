#include <iostream>
#include "Window.h"
#include "gl/glew.h"

void WindowClass::Create(glm::ivec2 size, std::string title)
{
	mSize = size;
	mTitle = title;
	mClosed = false;
	mWindow = SDL_CreateWindow(mTitle.data(), 100, 100, mSize.x, mSize.y, SDL_WINDOW_OPENGL);

	if (mWindow == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
		SDL_Quit();
		exit(1);
	}

	//setting the attributes for the opengl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//creating the context
	mContext = SDL_GL_CreateContext(mWindow);

	//if is invalid
	if (mContext == nullptr)
	{
		SDL_DestroyWindow(mWindow);
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
		SDL_Quit();
		exit(1);
	}
}

void WindowClass::Update() { /*if input manager esc -> mClosed = true*/}

void WindowClass::Clear()
{
	//clearing the window to black and the depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowClass::ShutDown()
{
	//freeing the openGL context
	SDL_GL_DeleteContext(mContext);
	//freeing the sdl window
	SDL_DestroyWindow(mWindow);
}

void WindowClass::CloseWindow() { mClosed = true; }

bool WindowClass::IsClosed() const { return mClosed; }

SDL_Window* WindowClass::GetSDLWindow() const { return nullptr; }

SDL_GLContext WindowClass::GetSDLContext() const { return SDL_GLContext(); }

glm::ivec2 WindowClass::GetViewport() const { return mSize; }
