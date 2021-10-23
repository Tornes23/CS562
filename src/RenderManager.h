#pragma once
#include <map>
#include <vector>
#include <string>
#include "json/json.hpp"
#include "Light.h"
#include "Shader.h"
#include "tinyglft/tiny_gltf.h"
#include "Model.h"
#include "GBuffer.h"
#include "FrameBuffer.h"
#include "Frustum.h"

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
		Regular,
		DepthPass
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
	void LoadDirectional(const nlohmann::json& directional);
	void LoadShaders(bool reload = false);
	void FreeShaders();
	void AddLight();
	
	void Render();
	void Display();
	void DisplayShadowMaps();
	void ClearBuffer();
	void GeometryStage();
	void LightingStage();
	void AmbientPass();
	void ShadowPass();
	void LightingPass();
	void PostProcessStage();
	void CreateFrusta();

	void RenderNode(Model& model, const tinygltf::Node& node);
	void RenderMesh(Model& model, const tinygltf::Mesh& mesh);
	
	void ExtractLuminence();
	void BlurTexture(bool horizontal = false, bool first_pass = false);
	void BindGTextures();
	ShaderProgram& GetShader(const RenderMode& mode);
	GLuint GenTexture(const glm::ivec2& size, bool high_precision = false, bool depth = false);
	GLuint GenDepthTexture(const glm::ivec2& size);
	Color GenRandomCol();
	glm::vec3 GenRandomPos();
	bool LightsAnimated() const;

private:
	const std::string mShaderPath = "./data/shaders/";
	std::vector<ShaderProgram> mShaders;
	GBuffer mGBuffer;
	Model* mScreenTriangle;
	DisplayTex mDisplay;
	FrameBuffer mFB;
	float mContrast;

	const int MAX_LIGHTS = 3000;
	bool mLightsAnimated;
	float mLightRad;
	float mLuminence;
	Color mAmbient;
	std::map<Light::LightType, std::vector<Light>> mLights;

	bool mBloom;
	int mBlurSamples;
	FrameBuffer mBloomBuffer;

	glm::ivec2 mShadowResolution;
	int mFrustaCount;
	int mPCFSamples;
	float mLambda;
	float mBlendDist;
	float mShadowBias;
	float mOccluderDist;
	std::vector<Frustum> mFrusta;

	RenderManagerClass() {}
};

#define RenderManager (RenderManagerClass::GetInstance())