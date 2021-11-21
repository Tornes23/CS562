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
#include "AOBuffer.h"


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
	White,
	AmbientOcclusion
};

enum DisplayTex
{
	Standar,
	Diffuse,
	Normal,
	Specular,
	Depth,
	AO
};

struct DeferredData
{
	void Init();
	void Edit();

	GBuffer mGBuffer;
	DisplayTex mDisplay;
};

struct LightData
{
	void Init();
	void Edit();

	const int MAX_LIGHTS = 3000;
	std::vector<Light> mLights;
	Color mAmbient;
	float mLightRad;
	bool mbActive;
};

struct BloomData
{
	void Init();
	void Edit();

	BloomBuffer mBB;//Bloom buffer
	bool mbActive;
	int mBlurSamples;
	float mLuminence;
};

struct DecalData
{
	enum DecalMode
	{
		Volume,
		Projected,
		Result,
	};
	void Init(GLuint diffuse, GLuint normal, GLuint specular);
	void Edit();

	std::vector<Decal> mDecals;
	DecalBuffer mDB;//Decal Buffer
	float mClipAngle;
	bool mbActive;
	DecalMode mDecalMode;
};

struct AOData
{
	void Init();
	void Edit();


	bool mbActive;
	int mDirectionNum;
	int mSteps;
	float mBias;
	float mRadius;
	float mAttenuation;
	float mScale;
	int mBlurPasses;
	float mRangeSigma;
	AOBuffer mAOBuffer;

};

struct RenderData
{
	void Init();
	void Edit();

	std::map<RenderMode, ShaderProgram> mShaders;
	Model* mScreenTriangle;
	RenderMode mMode;
	FrameBuffer mFB;//Frame Buffer	
	FrameBuffer mDisplayBuffer;//Frame Buffer	
	float mContrast;
};

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

	void AOStage();
	void AOPass();

	void LightPass();
	void AmbientPass();
	void RenderLights();
	void LightingStage();

	void PostProcessStage();
	void ExtractLuminence();
	void BlurTexture(bool horizontal = false, bool first_pass = false, bool gaussian = true);
	void BlendBlur();

	void RenderNode(Model& model, const tinygltf::Node& node);
	void RenderMesh(Model& model, const tinygltf::Mesh& mesh, glm::mat4x4& gltf_mat);
	
	ShaderProgram& GetShader();
	GLuint GenTexture(const glm::ivec2& size, bool high_precision = false);
	Color GenRandomCol();
	glm::vec3 GenRandomPos();
	bool LightsAnimated() const;

private:
	const std::string mShaderPath = "./data/shaders/";
	RenderData mRenderData;
	DeferredData mDeferredData;
	LightData mLightData;
	BloomData mBloomData;
	DecalData mDecalsData;
	AOData mAOData;


	RenderManagerClass() {}
};

#define RenderManager (RenderManagerClass::GetInstance())