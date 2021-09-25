#include "sdl2/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Window.h"
#include "Editor.h"

void EditorClass::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(Window.GetSDLWindow(), Window.GetSDLContext());
	ImGui_ImplOpenGL3_Init("#version 130");
}

void EditorClass::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorClass::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(Window.GetSDLWindow());
	ImGui::NewFrame();
}

void EditorClass::ShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void EditorClass::HandleEvent(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}
