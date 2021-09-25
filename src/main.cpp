#include <iostream>
#include "sdl2/SDL.h"
#include "imgui/imgui.h"
#include "Framework.h"

#undef main
int main(char* argv[], int argc)
{
	//handle the console arguments if necesary

	Utils::LoadScene("./data/scenes/scene.json");
	//RenderManager.Initialize();
	Utils::InitSDL();
	InputManager.Initialize();
	Window.Create();
	Editor.Initialize();
	Utils::InitGL();
	bool test = true;
	while (!Window.IsClosed())
	{
		Editor.StartFrame();
		InputManager.StartFrame();

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
				InputManager.HandleEvent(event);
				break;
			}
		}

		Window.Update();
		GOManager.Update();
		Camera.Update();

		Window.Clear();
		ImGui::ShowDemoWindow(&test);
		//render objs
		Editor.Render();
		Window.SwapBuffers();

	}

	Window.ShutDown();
	Editor.ShutDown();
	SDL_Quit();

	return 0;
}