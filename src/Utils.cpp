#include <iostream>
#include <fstream>
#include "sdl2/SDL.h"
#include "gl/glew.h"
#include "Utils.h"
#include "Window.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "RenderManager.h"
#include "ResourceManager.h"

namespace Utils
{

	void InitSDL()
	{
		//initializing the SDL library to video only
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "Could not initialize SDL: " << SDL_GetError() << "\n";
			exit(1);
		}
	}

	void InitGL()
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

	void ParseShader(std::string& filename, std::string& code)
	{
		//stream to read from the file
		std::ifstream inFile(filename, std::ios::in);

		//checking if it was opened
		if (!inFile)
			std::cerr << "ERROR WHILE TRYING TO OPEN " << filename << " FILE\n";

		//reading the code from the file and adding it to the string
		while (inFile.good())
		{
			int c = inFile.get();
			if (!inFile.eof())
				code.push_back((char)c);
		}
		//closing the file
		inFile.close();
	}

	void LoadScene(const std::string& filename)
	{
		std::ifstream inFile(filename);

		nlohmann::json j;
		if (inFile.good() && inFile.is_open())
		{
			inFile >> j;
			nlohmann::json& lights = *j.find("lights");
			if (j.find("camera") != j.end())
				Camera.LoadCamera(*j.find("camera"));
			if (j.find("objects") != j.end())
				GOManager.LoadObjects(*j.find("objects"));
			if (j.find("lights") != j.end())
				RenderManager.LoadLights(*j.find("lights"));
		}

	}
	void LoadResources(const std::string& filename)
	{
		//loading the model
		ResourceManager.Load();
		//loading the level
		LoadScene(filename);
	}
}

bool& operator>>(const nlohmann::json& j, bool& val) { val = j; return val; }
short& operator>>(const nlohmann::json& j, short& val) { val = j; return val; }
unsigned short& operator>>(const nlohmann::json& j, unsigned short& val) { val = j; return val; }
int& operator>>(const nlohmann::json& j, int& val) { val = j; return val; }
unsigned int& operator>>(const nlohmann::json& j, unsigned int& val) { val = j; return val; }
float& operator>>(const nlohmann::json& j, float& val) { val = j; return val; }
double& operator>>(const nlohmann::json& j, double& val) { val = j; return val; }
glm::vec2& operator>>(const nlohmann::json& j, glm::vec2& val) { j["x"] >> val.x; j["y"] >> val.y; return val; }
glm::vec3& operator>>(const nlohmann::json& j, glm::vec3& val) { j["x"] >> val.x; j["y"] >> val.y; j["z"] >> val.z; return val; }
glm::vec4& operator>>(const nlohmann::json& j, glm::vec4& val) { j["x"] >> val.x; j["y"] >> val.y; j["z"] >> val.z; j["w"] >> val.w; return val; }
Color& operator>>(const nlohmann::json& j, Color& val) { val = { j["x"], j["y"], j["z"] }; return val; }
std::string& operator>>(const nlohmann::json& j, std::string& val) { val = j.get<std::string>(); return val; }
GameObject& operator>>(const nlohmann::json& j, GameObject& val) { j["mesh"] >> val.mMesh; j["translate"] >> val.mPos; j["rotate"] >> val.mRotation; j["scale"] >> val.mScale; return val; }
Light& operator>>(const nlohmann::json& j, Light& val) { j["position"] >> val.mPos; j["color"] >> val.mColor; j["radius"] >> val.mRadius; return val; }