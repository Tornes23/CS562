#include <iostream>
#include "sdl2/SDL.h"
#include "imgui/imgui.h"
#include "Framework.h"

#undef main
int main(char* argv[], int argc)
{
	//handle the console arguments if necesary
	Utils::InitSDL();
	Window.Create();
	Utils::InitGL();
	InputManager.Initialize();
	Editor.Initialize();
	RenderManager.Initialize();
	Utils::LoadResources("./data/scenes/scene.json");

	bool test = true;
	while (!Window.IsClosed())
	{
		Editor.StartFrame();
		InputManager.StartFrame();

		InputManager.HandleEvents();
		
		Window.Update();
		GOManager.Update();
		Camera.Update();

		Window.Clear();
		ImGui::ShowDemoWindow(&test);
		RenderManager.Render();
		Editor.Render();
		Window.SwapBuffers();

	}

	Window.ShutDown();
	Editor.ShutDown();
	SDL_Quit();

	return 0;
}