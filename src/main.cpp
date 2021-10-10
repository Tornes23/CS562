#include <iostream>
#include "sdl2/SDL.h"
#include "imgui/imgui.h"
#include "Framework.h"

#undef main
int main(char* argv[], int argc)
{
	//std::string to_load = "./data/scenes/sceneSkull.json";
	//std::string to_load = "./data/scenes/sceneSphere.json";
	std::string to_load = "./data/scenes/scene.json";
	std::string output;
	if (argc == 2)
	{
		std::cerr << "2 outputs\n";
		to_load = argv[0];
		output = argv[1];
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
		Editor.Render();
		Window.SwapBuffers();

		if (argc == 2)
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