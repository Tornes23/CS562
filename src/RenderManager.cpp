#include <limits>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>
#include <imgui/imgui.h>
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "Window.h"
#include "JSON.h"
#include "Utils.h"
#include "Camera.h"
#include "InputManager.h"

void RenderManagerClass::Initialize()
{
	LoadShaders();
	mRenderData.Init();
	mDeferredData.Init();
	mLightData.Init();
	mBloomData.Init();
	mDecalsData.Init(mDeferredData.mGBuffer.mDiffuseBuffer, mDeferredData.mGBuffer.mNormalBuffer, mDeferredData.mGBuffer.mSpecularBuffer);
	mAOData.Init();
}

void RenderManagerClass::Update()
{
	Edit();

	if (KeyDown(Key::F5))
		LoadShaders(true);

	bool ctrl = KeyDown(Key::Control);

	if (KeyDown(Key::Num1) && !ctrl)
		mDeferredData.mDisplay = DisplayTex::Standar;
	if (KeyDown(Key::Num2) && !ctrl)
		mDeferredData.mDisplay = DisplayTex::Diffuse;
	if (KeyDown(Key::Num3) && !ctrl)
		mDeferredData.mDisplay = DisplayTex::Normal;
	if (KeyDown(Key::Num4) && !ctrl)
		mDeferredData.mDisplay = DisplayTex::Specular;
	if (KeyDown(Key::Num5) && !ctrl)
		mDeferredData.mDisplay = DisplayTex::Depth;
	if (KeyDown(Key::Num6) && !ctrl)
		mDeferredData.mDisplay = DisplayTex::AO;

	if (KeyDown(Key::Num1) && ctrl)
		mDecalsData.mDecalMode = DecalData::DecalMode::Volume;
	if (KeyDown(Key::Num2) && ctrl)
		mDecalsData.mDecalMode = DecalData::DecalMode::Projected;
	if (KeyDown(Key::Num3) && ctrl)
		mDecalsData.mDecalMode = DecalData::DecalMode::Result;



	for (auto& it : mLightData.mLights)
		it.Update();
		
}

void RenderManagerClass::Edit()
{
	//creating a window
	if (!ImGui::Begin("Render Options"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	mRenderData.Edit();
	mDeferredData.Edit();
	mLightData.Edit();
	mBloomData.Edit();
	mDecalsData.Edit();
	mAOData.Edit();

	ImGui::End();
}

void RenderManagerClass::LoadLights(const nlohmann::json& lights)
{
	mLightData.mLights.clear();
	for (auto it = lights.begin(); it != lights.end(); it++)
	{
		Light light;
		nlohmann::json object = *it;
		//load light
		object >> light;
		light.mInitialY = light.mPos.y;
		light.mModel = ResourceManager.GetResource<Model>("Sphere.gltf");
		mLightData.mLightRad += light.mRadius;
		//load mesh
		mLightData.mLights.push_back(light);
	}

	mLightData.mLightRad /= mLightData.mLights.size();
	 
	mRenderData.mScreenTriangle = ResourceManager.GetResource<Model>("ScreenTriangle.gltf");
}

void RenderManagerClass::LoadDecals(const nlohmann::json& decals)
{
	mDecalsData.mDecals.clear();
	for (auto it = decals.begin(); it != decals.end(); it++)
	{
		Decal decal;
		nlohmann::json object = *it;
		//load decal
		object >> decal;

		decal.GenM2W();
		//load mesh
		mDecalsData.mDecals.push_back(decal);
	}
}

void RenderManagerClass::LoadShaders(bool reload)
{
	if (reload)
		FreeShaders();

	mRenderData.mShaders[RenderMode::Geometry] = ShaderProgram("./data/shaders/GeometryStg.vert", "./data/shaders/GeometryStg.frag");
	mRenderData.mShaders[RenderMode::Lighting] = ShaderProgram("./data/shaders/LightingStg.vert", "./data/shaders/LightingStg.frag");
	mRenderData.mShaders[RenderMode::Ambient] = ShaderProgram("./data/shaders/Ambient.vert", "./data/shaders/Ambient.frag");
	mRenderData.mShaders[RenderMode::Luminence] = ShaderProgram("./data/shaders/Luminence.vert", "./data/shaders/Luminence.frag");
	mRenderData.mShaders[RenderMode::Blur] = ShaderProgram("./data/shaders/Blur.vert", "./data/shaders/Blur.frag");
	mRenderData.mShaders[RenderMode::Decals] = ShaderProgram("./data/shaders/Decal.vert", "./data/shaders/Decal.frag");
	mRenderData.mShaders[RenderMode::Regular] = ShaderProgram("./data/shaders/Regular.vert", "./data/shaders/Regular.frag");
	mRenderData.mShaders[RenderMode::Blend] = ShaderProgram("./data/shaders/Blend.vert", "./data/shaders/Blend.frag");
	mRenderData.mShaders[RenderMode::White] = ShaderProgram("./data/shaders/White.vert", "./data/shaders/White.frag");
	mRenderData.mShaders[RenderMode::AmbientOcclusion] = ShaderProgram("./data/shaders/AmbientOcclusion.vert", "./data/shaders/AmbientOcclusion.frag");
}

void RenderManagerClass::FreeShaders()
{
	//for each shader destroy
	for (auto& it : mRenderData.mShaders)
		it.second.Free();

	mRenderData.mShaders.clear();
}

void RenderManagerClass::AddLight()
{
	Light temp;
	temp.mPos = GenRandomPos();
	temp.mColor = GenRandomCol();
	temp.mRadius = mLightData.mLightRad;
	temp.mModel = ResourceManager.GetResource<Model>("Sphere.gltf");
	temp.mInitialY = temp.mPos.y;

	mLightData.mLights.push_back(temp);
}

void RenderManagerClass::Render()
{
	GeometryStage();

	if(mDecalsData.mbActive)
		DecalStage();

	if(mAOData.mbActive)
		AOStage();

	LightingStage();

	if(mBloomData.mbActive)
		PostProcessStage();

	Display();
}

void RenderManagerClass::GeometryStage()
{
	mRenderData.mMode = RenderMode::Geometry;
	auto objs = GOManager.GetObjs();
	//binding GBuffer
	mDeferredData.mGBuffer.Bind();
	ClearBuffer();
	//get shader program
	ShaderProgram& shader = mRenderData.mShaders[RenderMode::Geometry];
	shader.Use();
	for (auto& it : objs)
	{
		it.mModel->BindVAO();
		//set uniforms in shader
		glm::mat4x4 mv = Camera.GetCameraMat() * it.mM2W;
		glm::mat4x4 mvp = Camera.GetProjection() * mv;
		glm::mat4x4 m2v_normal = glm::transpose(glm::inverse(mv));
		shader.SetMatUniform("MV", &mv[0][0]);
		shader.SetMatUniform("MVP", &mvp[0][0]);
		shader.SetMatUniform("m2v_normal", &m2v_normal[0][0]);

		const tinygltf::Scene& scene = it.mModel->GetGLTFModel().scenes[it.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*it.mModel, it.mModel->GetGLTFModel().nodes[scene.nodes[i]]);

		//unbinding the VAOs
		glBindVertexArray(0);
	}

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::DecalStage()
{
	if (mDecalsData.mDecals.empty())
		return;

	mRenderData.mMode = RenderMode::Decals;
	glm::vec2 size = Window.GetViewport();
	mDeferredData.mGBuffer.BindReadBuffer();
	mDecalsData.mDB.BindDrawBuffer();
	glBlitFramebuffer(0, 0, static_cast<GLint>(size.x), static_cast<GLint>(size.y), 0, 0,
		static_cast<GLint>(size.x), static_cast<GLint>(size.y), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	mDecalsData.mDB.UseRenderBuffer();
	
	//get shader program
	ShaderProgram& shader = mRenderData.mShaders[RenderMode::Decals];
	shader.Use();
	mDeferredData.mGBuffer.BindDepthTexture();

	//disabling writing ono the depth buffer
	glDepthFunc(GL_GREATER);
	glCullFace(GL_FRONT);

	for (auto& decal : mDecalsData.mDecals)
	{
		//binding the screen triangle
		decal.mModel->BindVAO();
		//set uniforms in shader
		glm::mat4x4 mv = Camera.GetCameraMat() * decal.mM2W;
		glm::mat4x4 mvp = Camera.GetProjection() * mv;
		glm::mat4x4 invP = glm::inverse(Camera.GetProjection());
		glm::mat4x4 invV = glm::inverse(Camera.GetCameraMat());
		glm::mat4x4 invM2W = glm::inverse(decal.mM2W);

		shader.SetVec2Uniform("Size", Window.GetViewport());

		shader.SetMatUniform("MVP", &mvp[0][0]);
		shader.SetMatUniform("invP", &invP[0][0]);
		shader.SetMatUniform("invV", &invV[0][0]);
		shader.SetMatUniform("invM", &invM2W[0][0]);
		shader.SetMatUniform("MV", &mv[0][0]);
		shader.SetFloatUniform("ClipAngle", mDecalsData.mClipAngle);

		shader.SetIntUniform("Mode", static_cast<int>(mDecalsData.mDecalMode));
		decal.SetUniforms();
		//rendering the screen triangle
		const tinygltf::Scene& scene = decal.mModel->GetGLTFModel().scenes[decal.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*decal.mModel, decal.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
	
		//unbinding the VAOs
		glBindVertexArray(0);
	}

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::AOStage()
{
	//mAOData.mAOBuffer.UseRenderBuffer();
	//ClearBuffer();
	//computing the AO Texture
	AOPass();
	mAOData.mAOBuffer.UseRenderBuffer();
	ClearBuffer();
	//Blurring the AOTexture
	mRenderData.mMode = RenderMode::Blur;
	bool horizontal = true;
	for (int i = 0; i < mAOData.mBlurPasses; i++)
	{
		if (i == 0)
			BlurTexture(horizontal, true, false);
		else
			BlurTexture(horizontal, false, false);
	
		horizontal = !horizontal;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::AOPass()
{
	glm::vec2 size = Window.GetViewport();
	mRenderData.mMode = RenderMode::AmbientOcclusion;
	mRenderData.mFB.BindDrawBuffer();
	//mAOData.mAOBuffer.BindDrawBuffer();
	glBlitFramebuffer(0, 0, static_cast<GLint>(size.x), static_cast<GLint>(size.y), 0, 0,
		static_cast<GLint>(size.x), static_cast<GLint>(size.y), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	mRenderData.mFB.UseRenderBuffer();
	//mAOData.mAOBuffer.UseRenderBuffer();

	//get shader program
	ShaderProgram& shader = GetShader();
	shader.Use();
	//binding the required gbuffer textures
	mDeferredData.mGBuffer.BindPositionTexture();
	//binding the screen triangle
	mRenderData.mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 p = Camera.GetProjection();
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetMatUniform("Proj", &p[0][0]);

	shader.SetIntUniform("mDirectionNum", mAOData.mDirectionNum);
	shader.SetIntUniform("mSteps", mAOData.mSteps);
	shader.SetFloatUniform("mBias", mAOData.mBias);
	shader.SetFloatUniform("mRadius", mAOData.mRadius);
	shader.SetFloatUniform("mAttenuation", mAOData.mAttenuation);
	shader.SetFloatUniform("mScale", mAOData.mScale);
	shader.SetFloatUniform("Seed", glm::linearRand(0.0F, 1000.0F));

	//disabling depth writing
	glDepthMask(GL_FALSE);
	//rendering the screen triangle
	const tinygltf::Scene& scene = mRenderData.mScreenTriangle->GetGLTFModel().scenes[mRenderData.mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mRenderData.mScreenTriangle, mRenderData.mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	//disabling writing onto the depth buffer
	glDepthMask(GL_TRUE);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::LightingStage()
{
	if (mLightData.mLights.empty())
		return;

	AmbientPass();
	LightPass();
	RenderLights();
}

void RenderManagerClass::LightPass()
{
	mRenderData.mMode = RenderMode::Lighting;
	glm::vec2 size = Window.GetViewport();
	mDeferredData.mGBuffer.BindReadBuffer();
	mRenderData.mDisplayBuffer.BindDrawBuffer();
	glBlitFramebuffer(0, 0, static_cast<GLint>(size.x), static_cast<GLint>(size.y), 0, 0,
		static_cast<GLint>(size.x), static_cast<GLint>(size.y), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	mRenderData.mDisplayBuffer.UseRenderBuffer();

	//get shader program
	ShaderProgram& shader = mRenderData.mShaders[RenderMode::Lighting];
	shader.Use();
	mDeferredData.mGBuffer.BindTextures();
	//Diabling the back face culling
	glCullFace(GL_FRONT);
	//SET BLENDING TO ADDITIVE
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	//disabling writing ono the depth buffer
	glDepthFunc(GL_GREATER);
	glDepthMask(GL_FALSE);

	for (auto& light : mLightData.mLights)
	{
		//binding the screen triangle
		light.mModel->BindVAO();
		//set uniforms in shader
		glm::mat4x4 mv = Camera.GetCameraMat() * light.mM2W;
		glm::mat4x4 mvp = Camera.GetProjection() * mv;
		glm::mat4x4 invP = glm::inverse(Camera.GetProjection());
		shader.SetMatUniform("MV", &mv[0][0]);
		shader.SetMatUniform("MVP", &mvp[0][0]);
		shader.SetMatUniform("invP", &invP[0][0]);
		shader.SetVec2Uniform("Size", Window.GetViewport());
		light.SetUniforms("mLight", &shader);

		//rendering the screen triangle
		const tinygltf::Scene& scene = light.mModel->GetGLTFModel().scenes[light.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*light.mModel, light.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::RenderLights()
{
	mRenderData.mMode = RenderMode::White;
	mRenderData.mDisplayBuffer.UseRenderBuffer();
	//get shader program
	ShaderProgram& shader = mRenderData.mShaders[RenderMode::White];
	shader.Use();
	//SET BLENDING TO ADDITIVE
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	for (auto& light : mLightData.mLights)
	{
		//binding the screen triangle
		light.mModel->BindVAO();
		//set uniforms in shader
		glm::mat4x4 m2w = glm::translate(glm::mat4x4(1.0F), light.mPos);
		m2w = glm::scale(m2w, glm::vec3(2.0F));
		glm::mat4x4 mvp = Camera.GetProjection() * Camera.GetCameraMat() * m2w;
		shader.SetMatUniform("MVP", &mvp[0][0]);

		//rendering the screen triangle
		const tinygltf::Scene& scene = light.mModel->GetGLTFModel().scenes[light.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*light.mModel, light.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::ExtractLuminence()
{
	mRenderData.mMode = RenderMode::Luminence;
	mRenderData.mFB.UseRenderBuffer();
	ClearBuffer();
	//get shader program
	ShaderProgram& shader = GetShader();
	shader.Use();
	//binding the screen triangle
	mRenderData.mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetFloatUniform("LumThreshold", mBloomData.mLuminence);

	//bind the necessary textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mRenderData.mDisplayBuffer.GetRenderTexture());
	glUniform1i(0, 0);
	//rendering the screen triangle
	const tinygltf::Scene& scene = mRenderData.mScreenTriangle->GetGLTFModel().scenes[mRenderData.mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mRenderData.mScreenTriangle, mRenderData.mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::BlurTexture(bool horizontal, bool first_pass, bool gaussian)
{
	//get shader program
	ShaderProgram& shader = mRenderData.mShaders[RenderMode::Blur];
	shader.Use();
	//binding the screen triangle
	mRenderData.mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetBoolUniform("HorizontalPass", horizontal);
	shader.SetBoolUniform("Gaussian", gaussian);
	shader.SetFloatUniform("RangeSigma", mAOData.mRangeSigma);

	glActiveTexture(GL_TEXTURE0);
	if(first_pass)
		glBindTexture(GL_TEXTURE_2D, mRenderData.mFB.GetRenderTexture());
	else
		glBindTexture(GL_TEXTURE_2D, gaussian ? mBloomData.mBB.GetLuminenceTexture(!horizontal) : mAOData.mAOBuffer.GetAOTexture(!horizontal));

	glUniform1i(0, 0);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mRenderData.mScreenTriangle->GetGLTFModel().scenes[mRenderData.mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mRenderData.mScreenTriangle, mRenderData.mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);

}

void RenderManagerClass::AmbientPass()
{
	mRenderData.mMode = RenderMode::Ambient;
	mRenderData.mDisplayBuffer.UseRenderBuffer();
	ClearBuffer();

	//get shader program
	ShaderProgram& shader = GetShader();
	shader.Use();
	//binding the screen triangle
	mRenderData.mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetColorUniform("Ambient", mLightData.mAmbient);
	shader.SetBoolUniform("AO", mAOData.mbActive);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDeferredData.mGBuffer.mDiffuseBuffer);
	glUniform1i(0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mAOData.mAOBuffer.GetAOTexture());
	glUniform1i(1, 1);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mRenderData.mScreenTriangle->GetGLTFModel().scenes[mRenderData.mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mRenderData.mScreenTriangle, mRenderData.mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::PostProcessStage()
{
	ExtractLuminence();
	mBloomData.mBB.UseRenderBuffer();
	ClearBuffer();

	mRenderData.mMode = RenderMode::Blur;
	bool horizontal = true;
	for (int i = 0; i < mBloomData.mBlurSamples; i++)
	{
		if(i == 0)
			BlurTexture(horizontal, true);
		else
			BlurTexture(horizontal);

		horizontal = !horizontal;
	}

	//blend blurred with standar
	BlendBlur();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::BlendBlur()
{
	mRenderData.mMode = RenderMode::Blend;
	mRenderData.mFB.UseRenderBuffer();
	ClearBuffer();

	//get shader program
	ShaderProgram& shader = mRenderData.mShaders[RenderMode::Blend];
	shader.Use();

	//binding the screen triangle
	mRenderData.mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mRenderData.mDisplayBuffer.GetRenderTexture());
	glUniform1i(0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mBloomData.mBB.GetLuminenceTexture());
	glUniform1i(1, 1);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mRenderData.mScreenTriangle->GetGLTFModel().scenes[mRenderData.mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mRenderData.mScreenTriangle, mRenderData.mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderManagerClass::Display()
{
	mRenderData.mMode = RenderMode::Regular;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ClearBuffer();
	//get shader program
	ShaderProgram& shader = mRenderData.mShaders[RenderMode::Regular];
	shader.Use();

	//binding the screen triangle
	mRenderData.mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	switch (mDeferredData.mDisplay)
	{
	case DisplayTex::Diffuse:
		glBindTexture(GL_TEXTURE_2D, mDeferredData.mGBuffer.mDiffuseBuffer);
		break;
	case DisplayTex::Normal:
		glBindTexture(GL_TEXTURE_2D, mDeferredData.mGBuffer.mNormalBuffer);
		break;
	case DisplayTex::Specular:
		glBindTexture(GL_TEXTURE_2D, mDeferredData.mGBuffer.mSpecularBuffer);
		break;
	case DisplayTex::Depth:
		glBindTexture(GL_TEXTURE_2D, mDeferredData.mGBuffer.mDepth);
		break;
	case DisplayTex::AO:
		glBindTexture(GL_TEXTURE_2D, mAOData.mAOBuffer.GetAOTexture());
		break;
	default:
		glBindTexture(GL_TEXTURE_2D, mBloomData.mbActive ? mRenderData.mFB.GetRenderTexture() : mRenderData.mDisplayBuffer.GetRenderTexture());
		break;
	}
	glUniform1i(0, 0);

	bool depth = mDeferredData.mDisplay == DisplayTex::Depth ? true : false;
	shader.SetBoolUniform("Depth", depth);
	if(depth)
		shader.SetFloatUniform("Contrast", mRenderData.mContrast);


	//rendering the screen triangle
	const tinygltf::Scene& scene = mRenderData.mScreenTriangle->GetGLTFModel().scenes[mRenderData.mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mRenderData.mScreenTriangle, mRenderData.mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
}

void RenderManagerClass::ClearBuffer()
{
	glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManagerClass::RenderNode(Model& model, const tinygltf::Node& node)
{
	const tinygltf::Model& tiny_model = model.GetGLTFModel();
	glm::mat4x4 mat(1.0F);

	if (!node.translation.empty())
		mat = mat * glm::translate(glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
	if (!node.rotation.empty())
	{
		mat = mat * glm::rotate(-glm::radians((float)node.rotation[0]), glm::vec3(1, 0, 0));
		mat = mat * glm::rotate(-glm::radians((float)node.rotation[1]), glm::vec3(0, 1, 0));
		mat = mat * glm::rotate(-glm::radians((float)node.rotation[2]), glm::vec3(0, 0, 1));
	}
	if(!node.scale.empty())
		mat = mat * glm::scale(glm::vec3(node.scale[0], node.scale[1], node.scale[2]));

	if((node.mesh >= 0) && (node.mesh < tiny_model.meshes.size()))
		RenderMesh(model, tiny_model.meshes[node.mesh], mat);

	for (size_t i = 0; i < node.children.size(); i++) 
		RenderNode(model, tiny_model.nodes[node.children[i]]);
}

void RenderManagerClass::RenderMesh(Model& model, const tinygltf::Mesh& mesh, glm::mat4x4& gltf_mat)
{
	const tinygltf::Model& tiny_model = model.GetGLTFModel();

	for (size_t i = 0; i < mesh.primitives.size(); i++) 
	{
		tinygltf::Primitive primitive = mesh.primitives[i];
		tinygltf::Accessor indexAccessor = tiny_model.accessors[primitive.indices];

		for (auto& attrib : primitive.attributes)
		{
			tinygltf::Accessor accessor = tiny_model.accessors[attrib.second];
			int byteStride =
				accessor.ByteStride(tiny_model.bufferViews[accessor.bufferView]);
			glBindBuffer(GL_ARRAY_BUFFER, model.mVBOs[accessor.bufferView]);

			int size = 1;
			if (accessor.type != TINYGLTF_TYPE_SCALAR) {
				size = accessor.type;
			}

			int vaa = -1;
			if (attrib.first.compare("POSITION") == 0) vaa = 0;
			if (attrib.first.compare("NORMAL") == 0) vaa = 1;
			if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
			if (vaa > -1) {
				glEnableVertexAttribArray(vaa);
				glVertexAttribPointer(vaa, size, accessor.componentType,
					accessor.normalized ? GL_TRUE : GL_FALSE,
					byteStride, BUFFER_OFFSET(accessor.byteOffset));
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.mVBOs[indexAccessor.bufferView]);

		//set correct material active
		model.SetMaterialActive(mesh.primitives[i].material);	

		auto& shader = GetShader();
		shader.SetMatUniform("GLTF", &gltf_mat[0][0]);

		glDrawElements(primitive.mode, static_cast<GLsizei>(indexAccessor.count), indexAccessor.componentType,
						BUFFER_OFFSET(indexAccessor.byteOffset));
	}
}

ShaderProgram& RenderManagerClass::GetShader() { return mRenderData.mShaders[mRenderData.mMode]; }

GLuint RenderManagerClass::GenTexture(const glm::ivec2& size, bool high_precision)
{
	auto precision = high_precision ? GL_RGBA16F : GL_RGBA;

	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, precision, size.x, size.y, 0, GL_RGBA, high_precision ? GL_FLOAT : GL_UNSIGNED_BYTE, nullptr);
	//setting the parameters for the texture to clamp to the maximum
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return handle;
}

Color RenderManagerClass::GenRandomCol() { return Color(glm::linearRand(0.0F ,1.0F), glm::linearRand(0.0F, 1.0F), glm::linearRand(0.0F, 1.0F), 1.0F); }

glm::vec3 RenderManagerClass::GenRandomPos() { return glm::vec3(glm::linearRand(-100.0F, 100.0F), glm::linearRand(-100.0F, 100.0F), glm::linearRand(-200.0F, 200.0F)); }

bool RenderManagerClass::LightsAnimated() const { return mLightData.mbActive;}

void DeferredData::Init()
{
	mGBuffer.Create();
	mDisplay = DisplayTex::Standar;

}

void DeferredData::Edit()
{
	//code to change the output
	int tex = static_cast<int>(mDisplay);

	const char* texture_options[6] = { "Standar", "Diffuse", "Normal", "Specular", "Depth", "Ambient Occlusion"};

	if (ImGui::Combo("Display Texture", &tex, texture_options, 6, 7))
	{
		switch (tex)
		{
		case 0:
			mDisplay = DisplayTex::Standar;
			break;
		case 1:
			mDisplay = DisplayTex::Diffuse;
			break;
		case 2:
			mDisplay = DisplayTex::Normal;
			break;
		case 3:
			mDisplay = DisplayTex::Specular;
			break;
		case 4:
			mDisplay = DisplayTex::Depth;
			break;
		case 5:
			mDisplay = DisplayTex::AO;
			break;
		}
	}
}

void LightData::Init()
{
	mAmbient = Color(0.6F);
	mbActive = false;
}

void LightData::Edit()
{
	if (ImGui::TreeNode("Light Data"))
	{
		ImGui::Checkbox("Animation", &mbActive);
		int lights = static_cast<int>(mLights.size());
		float ambient = mAmbient.GetColor()[0];
		ImGui::SliderFloat("Ambient Intensity", &ambient, 0, 1.0F);
		mAmbient = Color(ambient);

		ImGui::SliderInt("Light Count", &lights, 0, MAX_LIGHTS);

		if (lights > mLights.size())
		{
			while (mLights.size() < lights)
				RenderManager.AddLight();
		}
		else
		{
			while (mLights.size() > lights)
				mLights.pop_back();

		}

		bool rad = ImGui::SliderFloat("Light Radius", &mLightRad, 0.0F, 100.0F);

		if (mLightRad < 0.0F)
			mLightRad = 0.0F;

		if (rad)
		{
			for (auto& it : mLights)
				it.mRadius = mLightRad;
		}

		ImGui::TreePop();
	}
}

void BloomData::Init()
{
	mBB.Create();
	mbActive = false;
	mLuminence = 1.0F;
	mBlurSamples = 5;
}

void BloomData::Edit()
{
	if (ImGui::TreeNode("Bloom"))
	{
		ImGui::Checkbox("Bloom", &mbActive);
		ImGui::SliderInt("Bloom Smaples", &mBlurSamples, 0, 500);
		if (mBlurSamples < 0)
			mBlurSamples = 0;
		ImGui::SliderFloat("Luminence Threshold", &mLuminence, 0.0F, 5.0F);

		ImGui::TreePop();
	}
}

void DecalData::Init(GLuint diffuse, GLuint normal, GLuint specular)
{
	mDB.Create(diffuse, normal, specular);
	mDecalMode = DecalData::DecalMode::Result;
	mbActive= true;
	mClipAngle = 0.1F;
}

void DecalData::Edit()
{
	if (ImGui::TreeNode("Decals"))
	{
		ImGui::Checkbox("Decals", &mbActive);
		//code to change the output
		int dec = static_cast<int>(mDecalMode);

		const char* decal_options[3] = { "Volume", "Projected", "Result" };

		if (ImGui::Combo("Decal Stage", &dec, decal_options, 3, 4))
		{
			switch (dec)
			{
			case 0:
				mDecalMode = DecalData::DecalMode::Volume;
				break;
			case 1:
				mDecalMode = DecalData::DecalMode::Projected;
				break;
			case 2:
				mDecalMode = DecalData::DecalMode::Result;
				break;
			}
		}

		ImGui::SliderFloat("Clip Angle", &mClipAngle, 0.0F, 360.0F);

		ImGui::TreePop();
	}
}

void RenderData::Init()
{
	mMode = RenderMode::Regular;
	mFB.Create();
	mDisplayBuffer.Create();
	mContrast = 1.0F - 0.99784F;
}

void RenderData::Edit()
{
	ImGui::SliderFloat("Contrast", &mContrast, 0.0F, 1.0F);
	if (mContrast < 0.0F)
		mContrast = 0.0F;
	if (mContrast > 1.0F)
		mContrast = 1.0F;
}

void AOData::Init()
{
	mbActive = true;
	mDirectionNum = 6;
	mSteps = 6;
	mBias = 0.5F;
	mRadius = 5.0F;
	mAttenuation = 0.1F;
	mScale = 5.0F;
	mBlurPasses = 5;
	mRangeSigma = 0.5F;
	mAOBuffer.Create();
}

void AOData::Edit()
{
	if (ImGui::TreeNode("Ambient Occlusion"))
	{
		ImGui::Checkbox("Enable AO", &mbActive);

		ImGui::SliderInt("Direction Count", &mDirectionNum, 1, 50);
		ImGui::SliderInt("Steps", &mSteps, 0, 50);
		ImGui::SliderFloat("Bias", &mBias, 0.0F, 2.5F);
		ImGui::SliderFloat("Radius", &mRadius, 0.0F, 10.0F);
		ImGui::SliderFloat("Attenuation", &mAttenuation, 0.0F, 1.0F);
		ImGui::SliderFloat("Scale", &mScale, 0.0F, 25.0F);
		ImGui::SliderInt("Blur Passes", &mBlurPasses, 0, 100);
		ImGui::SliderFloat("Range Sigma", &mRangeSigma, 0.0F, 100.0F);

		ImGui::TreePop();
	}

}

