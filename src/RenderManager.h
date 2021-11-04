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
#include "BloomBuffer.h"
#include "DecalBuffer.h"
#include "Decal.h"

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
		Luminence,
		Blur,
		Decals,
		Regular,
		Blend,
		White
	};

	enum DisplayTex
	{
		Standar,
		Diffuse,
		Normal,
		Specular,
		Depth,
		DecalsTex
	};

	enum DecalMode
	{
		Volume,
		Projected,
		Result,
	};

	void Initialize();
	void Update();
	
	void Edit();
	void LoadLights(const nlohmann::json& lights);
	void LoadDecals(const nlohmann::json& decals);
	void LoadShaders(bool reload = false);
	void FreeShaders();
	void AddLight();
	
	void Render();
	void Display();
	void ClearBuffer();

	void GeometryStage();

	void DecalStage();

	void LightPass();
	void AmbientPass();
	void RenderLights();
	void LightingStage();

	void PostProcessStage();
	void ExtractLuminence();
	void BlurTexture(bool horizontal = false, bool first_pass = false);
	void BlendBlur();

	void RenderNode(Model& model, const tinygltf::Node& node);
	void RenderMesh(Model& model, const tinygltf::Mesh& mesh);
	
	ShaderProgram& GetShader(const RenderMode& mode);
	GLuint GenTexture(const glm::ivec2& size, bool high_precision = false);
	Color GenRandomCol();
	glm::vec3 GenRandomPos();
	bool LightsAnimated() const;

private:
	const std::string mShaderPath = "./data/shaders/";
	const int MAX_LIGHTS = 3000;
	std::vector<Light> mLights;
	std::vector<Decal> mDecals;
	std::map<RenderMode, ShaderProgram> mShaders;
	GBuffer mGBuffer;
	Model* mScreenTriangle;
	Color mAmbient;
	DisplayTex mDisplay;
	DecalMode mDecalMode;

	FrameBuffer mFB;//Frame Buffer	
	FrameBuffer mDisplayBuffer;//Frame Buffer	
	BloomBuffer mBB;//Bloom buffer
	DecalBuffer mDB;//Decal Buffer

	float mLightRad;
	float mLuminence;
	float mContrast;
	bool mBloom;
	bool mbUseDecals;
	bool mLightsAnimated;
	int mBlurSamples;
	RenderManagerClass() {}
};

#define RenderManager (RenderManagerClass::GetInstance())