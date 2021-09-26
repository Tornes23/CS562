#pragma once
#include <vector>
#include <string>
#include "json/json.hpp"
#include "Light.h"
#include "Shader.h"

class RenderManagerClass
{
public:
	//singleton stuff
	RenderManagerClass(RenderManagerClass const&) = delete;
	void operator=(RenderManagerClass const&) = delete;
	static RenderManagerClass& GetInstance()
	{
		static RenderManagerClass instance;
		return instance;
	}

	void Initialize();
	void LoadLights(const nlohmann::json& lights);
	void LoadShaders();
	void Render();

private:
	const std::string mShaderPath = "./data/shaders/";
	std::vector<Light> mLights;
	std::vector<ShaderProgram> mShaders;

	RenderManagerClass() {}
};

#define RenderManager (RenderManagerClass::GetInstance())