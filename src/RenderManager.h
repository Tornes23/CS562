#pragma once
#include <vector>
#include <string>
#include "json/json.hpp"
#include "Light.h"
#include "Shader.h"
#include "tinyglft/tiny_gltf.h"
#include "Model.h"
#include "GBuffer.h"

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

	enum RenderMode
	{
		Geometry,
		Lighting,
		Regular
	};

	void Initialize();
	void Update();
	
	void LoadLights(const nlohmann::json& lights);
	void LoadShaders(bool reload = false);
	void FreeShaders();
	
	void Render();
	void Display();
	void ClearBuffer();
	void GeometryStage();
	void LightingStage();
	void RenderNode(Model& model, const tinygltf::Node& node);
	void RenderMesh(Model& model, const tinygltf::Mesh& mesh);
	
	ShaderProgram& GetShader();
	GLuint GenTexture(const glm::ivec2& size, bool high_precision);

private:
	const std::string mShaderPath = "./data/shaders/";
	std::vector<Light> mLights;
	std::vector<ShaderProgram> mShaders;
	RenderMode mMode;
	GBuffer mGBuffer;
	
	RenderManagerClass() {}
};

#define RenderManager (RenderManagerClass::GetInstance())