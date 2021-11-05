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
	mGBuffer.Create();
	mFB.Create();
	mDisplayBuffer.Create();
	mBB.Create();
	mDB.Create(mGBuffer.mDiffuseBuffer, mGBuffer.mNormalBuffer, mGBuffer.mSpecularBuffer);
	LoadShaders();
	mAmbient = Color(0.6F);
	mDisplay = DisplayTex::Standar;
	mDecalMode = DecalMode::Result;
	mBloom = true;
	mbUseDecals = true;
	mLuminence = 1.0F;
	mClipAngle = 0.1F;
	mBlurSamples = 5;
	mLightsAnimated = false;
	mContrast = 1.0F - 0.99784F;
}

void RenderManagerClass::Update()
{
	Edit();

	if (KeyDown(Key::F5))
		LoadShaders(true);

	bool ctrl = KeyDown(Key::Control);

	if (KeyDown(Key::Num1) && !ctrl)
		mDisplay = DisplayTex::Standar;
	if (KeyDown(Key::Num2) && !ctrl)
		mDisplay = DisplayTex::Diffuse;
	if (KeyDown(Key::Num3) && !ctrl)
		mDisplay = DisplayTex::Normal;
	if (KeyDown(Key::Num4) && !ctrl)
		mDisplay = DisplayTex::Specular;
	if (KeyDown(Key::Num5) && !ctrl)
		mDisplay = DisplayTex::Depth;

	if (KeyDown(Key::Num1) && ctrl)
		mDecalMode = DecalMode::Volume;
	if (KeyDown(Key::Num2) && ctrl)
		mDecalMode = DecalMode::Projected;
	if (KeyDown(Key::Num3) && ctrl)
		mDecalMode = DecalMode::Result;



	for (auto& it : mLights)
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

	ImGui::Checkbox("Bloom", &mBloom);
	ImGui::SameLine();
	ImGui::Checkbox("Decals", &mbUseDecals);
	ImGui::DragInt("Bloom Smaples", &mBlurSamples);
	if (mBlurSamples < 0)
		mBlurSamples = 0;
	ImGui::DragFloat("Luminence Threshold", &mLuminence, 0.005F);
	ImGui::DragFloat("Contrast", &mContrast, 0.0001F, 0.0F, 1.0F);
	if (mContrast < 0.0F)
		mContrast = 0.0F;
	if (mContrast > 1.0F)
		mContrast = 1.0F;

	//code to change the output
	int tex = static_cast<int>(mDisplay);
	
	const char* texture_options[6] = { "Standar", "Diffuse", "Normal", "Specular", "Depth", "Decals"};
	
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
			mDisplay = DisplayTex::DecalsTex;
			break;
		}
	}

	if (ImGui::TreeNode("Light Data"))
	{
		ImGui::Checkbox("Animation", &mLightsAnimated);
		int lights = static_cast<int>(mLights.size());
		float ambient = mAmbient.GetColor()[0];
		ImGui::SliderFloat("Ambient Intensity", &ambient, 0, 1.0F);
		mAmbient = Color(ambient);

		ImGui::SliderInt("Light Count", &lights, 0, MAX_LIGHTS);

		if (lights > mLights.size())
		{
			while (mLights.size() < lights)
				AddLight();
		}
		else
		{
			while (mLights.size() > lights)
				mLights.pop_back();

		}

		bool rad = ImGui::DragFloat("Light Radius", &mLightRad, 1.0F, 0.0F);

		if (mLightRad < 0.0F)
			mLightRad = 0.0F;

		if (rad)
		{
			for (auto& it : mLights)
				it.mRadius = mLightRad;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Decals"))
	{
		//code to change the output
		int dec = static_cast<int>(mDecalMode);

		const char* decal_options[3] = { "Volume", "Projected", "Result" };

		if (ImGui::Combo("Decal Stage", &dec, decal_options, 3, 4))
		{
			switch (dec)
			{
			case 0:
				mDecalMode = DecalMode::Volume;
				break;
			case 1:
				mDecalMode = DecalMode::Projected;
				break;
			case 2:
				mDecalMode = DecalMode::Result;
				break;
			}
		}

		ImGui::DragFloat("Clip Angle", &mClipAngle, 0.02F, 0.0F);

		ImGui::TreePop();
	}


	ImGui::End();
}

void RenderManagerClass::LoadLights(const nlohmann::json& lights)
{
	mLights.clear();
	for (auto it = lights.begin(); it != lights.end(); it++)
	{
		Light light;
		nlohmann::json object = *it;
		//load light
		object >> light;
		light.mInitialY = light.mPos.y;
		light.mModel = ResourceManager.GetResource<Model>("Sphere.gltf");
		mLightRad += light.mRadius;
		//load mesh
		mLights.push_back(light);
	}

	mLightRad /= mLights.size();


	mScreenTriangle = ResourceManager.GetResource<Model>("ScreenTriangle.gltf");
}

void RenderManagerClass::LoadDecals(const nlohmann::json& decals)
{
	mDecals.clear();
	for (auto it = decals.begin(); it != decals.end(); it++)
	{
		Decal decal;
		nlohmann::json object = *it;
		//load decal
		object >> decal;

		decal.GenM2W();
		//load mesh
		mDecals.push_back(decal);
	}
}

void RenderManagerClass::LoadShaders(bool reload)
{
	if (reload)
		FreeShaders();

	mShaders[RenderMode::Geometry] = ShaderProgram("./data/shaders/GeometryStg.vert", "./data/shaders/GeometryStg.frag");
	mShaders[RenderMode::Lighting] = ShaderProgram("./data/shaders/LightingStg.vert", "./data/shaders/LightingStg.frag");
	mShaders[RenderMode::Ambient] = ShaderProgram("./data/shaders/Ambient.vert", "./data/shaders/Ambient.frag");
	mShaders[RenderMode::Luminence] = ShaderProgram("./data/shaders/Luminence.vert", "./data/shaders/Luminence.frag");
	mShaders[RenderMode::Blur] = ShaderProgram("./data/shaders/Blur.vert", "./data/shaders/Blur.frag");
	mShaders[RenderMode::Decals] = ShaderProgram("./data/shaders/Decal.vert", "./data/shaders/Decal.frag");
	mShaders[RenderMode::Regular] = ShaderProgram("./data/shaders/Regular.vert", "./data/shaders/Regular.frag");
	mShaders[RenderMode::Blend] = ShaderProgram("./data/shaders/Blend.vert", "./data/shaders/Blend.frag");
	mShaders[RenderMode::White] = ShaderProgram("./data/shaders/White.vert", "./data/shaders/White.frag");
}

void RenderManagerClass::FreeShaders()
{
	//for each shader destroy
	for (auto& it : mShaders)
		it.second.Free();

	mShaders.clear();
}

void RenderManagerClass::AddLight()
{
	Light temp;
	temp.mPos = GenRandomPos();
	temp.mColor = GenRandomCol();
	temp.mRadius = mLightRad;
	temp.mModel = ResourceManager.GetResource<Model>("Sphere.gltf");
	temp.mInitialY = temp.mPos.y;

	mLights.push_back(temp);
}

void RenderManagerClass::Render()
{
	GeometryStage();

	if(mbUseDecals)
		DecalStage();

	LightingStage();

	if(mBloom)
		PostProcessStage();

	Display();
}

void RenderManagerClass::GeometryStage()
{
	auto objs = GOManager.GetObjs();
	//binding GBuffer
	mGBuffer.Bind();
	ClearBuffer();
	//get shader program
	ShaderProgram& shader = mShaders[RenderMode::Geometry];
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
	glm::vec2 size = Window.GetViewport();
	mGBuffer.BindReadBuffer();
	mDB.BindDrawBuffer();
	glBlitFramebuffer(0, 0, static_cast<GLint>(size.x), static_cast<GLint>(size.y), 0, 0,
		static_cast<GLint>(size.x), static_cast<GLint>(size.y), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	mDB.UseRenderBuffer();
	
	//get shader program
	ShaderProgram& shader = mShaders[RenderMode::Decals];
	shader.Use();
	mGBuffer.BindDepthTexture();

	//disabling writing ono the depth buffer
	glDepthFunc(GL_GREATER);
	glCullFace(GL_FRONT);
	glDepthMask(GL_FALSE);

	for (auto& decal : mDecals)
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
		shader.SetFloatUniform("ClipAngle", mClipAngle);

		shader.SetIntUniform("Mode", static_cast<int>(mDecalMode));
		decal.SetUniforms();
		//rendering the screen triangle
		const tinygltf::Scene& scene = decal.mModel->GetGLTFModel().scenes[decal.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*decal.mModel, decal.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
	
		//unbinding the VAOs
		glBindVertexArray(0);
	}

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::LightingStage()
{
	AmbientPass();
	LightPass();
	RenderLights();
}

void RenderManagerClass::LightPass()
{
	glm::vec2 size = Window.GetViewport();
	mGBuffer.BindReadBuffer();
	mDisplayBuffer.BindDrawBuffer();
	glBlitFramebuffer(0, 0, static_cast<GLint>(size.x), static_cast<GLint>(size.y), 0, 0,
		static_cast<GLint>(size.x), static_cast<GLint>(size.y), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	mDisplayBuffer.UseRenderBuffer();

	//get shader program
	ShaderProgram& shader = mShaders[RenderMode::Lighting];
	shader.Use();
	mGBuffer.BindTextures();
	//Diabling the back face culling
	glCullFace(GL_FRONT);
	//SET BLENDING TO ADDITIVE
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	//disabling writing ono the depth buffer
	glDepthFunc(GL_GREATER);
	glDepthMask(GL_FALSE);

	for (auto& light : mLights)
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
	mDisplayBuffer.UseRenderBuffer();
	//get shader program
	ShaderProgram& shader = mShaders[RenderMode::White];
	shader.Use();
	//SET BLENDING TO ADDITIVE
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	for (auto& light : mLights)
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
	mFB.UseRenderBuffer();
	ClearBuffer();
	//get shader program
	ShaderProgram& shader = GetShader(RenderMode::Luminence);
	shader.Use();
	//binding the screen triangle
	mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetFloatUniform("LumThreshold", mLuminence);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDisplayBuffer.GetRenderTexture());
	glUniform1i(0, 0);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mScreenTriangle->GetGLTFModel().scenes[mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mScreenTriangle, mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::BlurTexture(bool horizontal, bool first_pass)
{
	//get shader program
	ShaderProgram& shader = mShaders[RenderMode::Blur];
	shader.Use();
	//binding the screen triangle
	mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetBoolUniform("HorizontalPass", horizontal);

	glActiveTexture(GL_TEXTURE0);
	if(first_pass)
		glBindTexture(GL_TEXTURE_2D, mFB.GetRenderTexture());
	else
		glBindTexture(GL_TEXTURE_2D, mBB.GetLuminenceTexture(!horizontal));

	glUniform1i(0, 0);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mScreenTriangle->GetGLTFModel().scenes[mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mScreenTriangle, mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);

}

void RenderManagerClass::AmbientPass()
{
	mDisplayBuffer.UseRenderBuffer();
	ClearBuffer();

	//get shader program
	ShaderProgram& shader = GetShader(RenderMode::Ambient);
	shader.Use();
	//binding the screen triangle
	mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetColorUniform("Ambient", mAmbient);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mGBuffer.mDiffuseBuffer);
	glUniform1i(0, 0);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mScreenTriangle->GetGLTFModel().scenes[mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mScreenTriangle, mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::PostProcessStage()
{
	ExtractLuminence();
	mBB.UseRenderBuffer();
	ClearBuffer();

	bool horizontal = true;
	for (int i = 0; i < mBlurSamples; i++)
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
	mFB.UseRenderBuffer();
	ClearBuffer();

	//get shader program
	ShaderProgram& shader = mShaders[RenderMode::Blend];
	shader.Use();

	//binding the screen triangle
	mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDisplayBuffer.GetRenderTexture());
	glUniform1i(0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mBB.GetLuminenceTexture());
	glUniform1i(1, 1);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mScreenTriangle->GetGLTFModel().scenes[mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mScreenTriangle, mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderManagerClass::Display()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ClearBuffer();
	//get shader program
	ShaderProgram& shader = mShaders[RenderMode::Regular];
	shader.Use();

	//binding the screen triangle
	mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	switch (mDisplay)
	{
	case DisplayTex::Diffuse:
		glBindTexture(GL_TEXTURE_2D, mGBuffer.mDiffuseBuffer);
		break;
	case DisplayTex::Normal:
		glBindTexture(GL_TEXTURE_2D, mGBuffer.mNormalBuffer);
		break;
	case DisplayTex::Specular:
		glBindTexture(GL_TEXTURE_2D, mGBuffer.mSpecularBuffer);
		break;
	case DisplayTex::Depth:
		glBindTexture(GL_TEXTURE_2D, mGBuffer.mDepth);
		break;
	case DisplayTex::DecalsTex:
		glBindTexture(GL_TEXTURE_2D, mDB.mNormalBuffer);
		break;
	default:
		glBindTexture(GL_TEXTURE_2D, mBloom ? mFB.GetRenderTexture() : mDisplayBuffer.GetRenderTexture());
		break;
	}
	glUniform1i(0, 0);

	bool depth = mDisplay == DisplayTex::Depth ? true : false;
	shader.SetBoolUniform("Depth", depth);
	if(depth)
		shader.SetFloatUniform("Contrast", mContrast);


	//rendering the screen triangle
	const tinygltf::Scene& scene = mScreenTriangle->GetGLTFModel().scenes[mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size() - 1; i++)
		RenderNode(*mScreenTriangle, mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

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

	if((node.mesh >= 0) && (node.mesh < tiny_model.meshes.size()))
		RenderMesh(model, tiny_model.meshes[node.mesh]);

	for (size_t i = 0; i < node.children.size(); i++) 
		RenderNode(model, tiny_model.nodes[node.children[i]]);
}

void RenderManagerClass::RenderMesh(Model& model, const tinygltf::Mesh& mesh)
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

		glDrawElements(primitive.mode, static_cast<GLsizei>(indexAccessor.count), indexAccessor.componentType,
						BUFFER_OFFSET(indexAccessor.byteOffset));
	}
}

ShaderProgram& RenderManagerClass::GetShader(const RenderMode& mode) { return mShaders[mode]; }

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

glm::vec3 RenderManagerClass::GenRandomPos() { return glm::vec3(glm::linearRand(-200.0F, 200.0F), glm::linearRand(-200.0F, 200.0F), glm::linearRand(-200.0F, 200.0F)); }

bool RenderManagerClass::LightsAnimated() const { return mLightsAnimated;}
