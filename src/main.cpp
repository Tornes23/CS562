#include <iostream>
#include "sdl2/SDL.h"
#include "imgui/imgui.h"
#include "Framework.h"

#undef main
int main(char* argv[], int argc)
{
	//handle the console arguments if necesary

	//InputManager.Initialize();
	//RenderManager.Initialize();
	Utils::InitSDL();
	Window.Create();
	Editor.Initialize();
	Utils::InitGL();
	//GOManager.LoadLevel("");
	bool test = true;
	while (!Window.IsClosed())
	{
		Editor.StartFrame();
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

		Window.Update();
		//update objs
		//update cam

		Window.Clear();
		ImGui::ShowDemoWindow(&test);
		//render objs
		Editor.Render();
		Window.SwapBuffers();

	}

	Window.ShutDown();
	Editor.ShutDown();
	SDL_Quit();

	std::cerr << "Hello World\n";
	return 0;
}