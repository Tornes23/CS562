#include <iostream>
#include "sdl2/SDL.h"
#include "imgui/imgui.h"
#include "Framework.h"

#undef main
int main(int argc, char* argv[])
{
	std::string to_load = "./data/scenes/scene.json";
	std::string output;
	if (argc == 3)
	{
		to_load = argv[1];
		output = argv[2];
	}

	//handle the console arguments if necesary
	Utils::InitSDL();
	Window.Create();
	Utils::InitGL();
	InputManager.Initialize();
	Editor.Initialize();
	RenderManager.Initialize();
	ResourceManager.LoadScene(to_load);

	while (!Window.IsClosed())
	{
		Window.StartFrame();
		Editor.StartFrame();
		InputManager.StartFrame();

		InputManager.HandleEvents();
		
		ResourceManager.Update();
		RenderManager.Update();
		Window.Update();
		GOManager.Update();
		Camera.Update();

		Window.Clear();
		RenderManager.Render();
		Utils::PerformanceGUI();
		Editor.Render();
		Window.SwapBuffers();

		if (argc == 3)
		{
			//print output
			Utils::SaveScreenshot(output);
			Window.CloseWindow();
		}

	}

	Window.ShutDown();
	Editor.ShutDown();
	SDL_Quit();

	return 0;
}