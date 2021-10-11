#pragma once
#include <vector>
#include <string>
#include "json/json.hpp"
#include "Light.h"
#include "Shader.h"
#include "tinyglft/tiny_gltf.h"
#include "Model.h"
#include "GBuffer.h"
#include "FrameBuffer.h"

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
		Ambient,
		Bloom,
		Regular
	};

	enum DisplayTex
	{
		Standar,
		Diffuse,
		Normal,
		Position,
		Specular,
		Depth
	};

	void Initialize();
	void Update();
	
	void Edit();
	void LoadLights(const nlohmann::json& lights);
	void LoadShaders(bool reload = false);
	void FreeShaders();
	void AddLight();
	
	void Render();
	void Display();
	void ClearBuffer();
	void GeometryStage();
	void LightingStage();
	void AmbientStage();
	void PostProcessStage();
	void BindGTextures();
	void RenderNode(Model& model, const tinygltf::Node& node);
	void RenderMesh(Model& model, const tinygltf::Mesh& mesh);
	
	ShaderProgram& GetShader(const RenderMode& mode);
	GLuint GenTexture(const glm::ivec2& size, bool high_precision = false);
	Color GenRandomCol();
	glm::vec3 GenRandomPos();

private:
	const std::string mShaderPath = "./data/shaders/";
	const int MAX_LIGHTS = 3000;
	std::vector<Light> mLights;
	std::vector<ShaderProgram> mShaders;
	RenderMode mMode;
	GBuffer mGBuffer;
	Model* mScreenTriangle;
	Color mAmbient;
	DisplayTex mDisplay;
	FrameBuffer mFB;
	float mLightRad;
	bool mBloom;
	bool mLightsAnimated;
	
	RenderManagerClass() {}
};

#define RenderManager (RenderManagerClass::GetInstance())