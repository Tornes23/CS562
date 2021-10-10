#include <glm/gtx/transform.hpp>
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
	LoadShaders();
	mAmbient = 0.02F;
	mDisplay = DisplayTex::Standar;
}

void RenderManagerClass::Update()
{
	if (KeyDown(Key::F5))
		LoadShaders(true);

	if (KeyDown(Key::Num1))
		mDisplay = DisplayTex::Standar;
	if (KeyDown(Key::Num2))
		mDisplay = DisplayTex::Diffuse;
	if (KeyDown(Key::Num3))
		mDisplay = DisplayTex::Normal;
	if (KeyDown(Key::Num4))
		mDisplay = DisplayTex::Position;
	if (KeyDown(Key::Num5))
		mDisplay = DisplayTex::Specular;
	//if (KeyDown(Key::Num6))
	//	mDisplay = DisplayTex::Depth;

	for (auto& it : mLights)
		it.Update();
		
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
		light.mModel = ResourceManager.GetResource<Model>("Sphere.gltf");
		//load mesh
		mLights.push_back(light);
	}

	mScreenTriangle = ResourceManager.GetResource<Model>("ScreenTriangle.gltf");
}

void RenderManagerClass::LoadShaders(bool reload)
{
	if (reload)
		FreeShaders();

	mShaders.push_back(ShaderProgram("./data/shaders/GeometryStg.vert", "./data/shaders/GeometryStg.frag"));
	mShaders.push_back(ShaderProgram("./data/shaders/LightingStg.vert", "./data/shaders/LightingStg.frag"));
	mShaders.push_back(ShaderProgram("./data/shaders/Ambient.vert", "./data/shaders/Ambient.frag"));
	mShaders.push_back(ShaderProgram("./data/shaders/Bloom.vert", "./data/shaders/Bloom.frag"));
	mShaders.push_back(ShaderProgram("./data/shaders/Regular.vert", "./data/shaders/Regular.frag"));
}

void RenderManagerClass::FreeShaders()
{
	//for each shader destroy
	for (auto& it : mShaders)
		it.Free();
}

void RenderManagerClass::Render()
{
	if(mDisplay == DisplayTex::Standar)
	{
		GeometryStage();
		LightingStage();
		AmbientStage();
		//Bloom(RenderMode::Bloom);
	}

	Display();
}

void RenderManagerClass::GeometryStage()
{
	auto objs = GOManager.GetObjs();
	//binding GBuffer
	mGBuffer.Bind();
	ClearBuffer();
	//get shader program
	ShaderProgram& shader = mShaders[static_cast<size_t>(RenderMode::Geometry)];
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
	}

	glUseProgram(0);
}

void RenderManagerClass::LightingStage()
{
	mFB.UseRenderBuffer();
	mFB.ClearColorBuffer();
	mFB.ClearDepthBuffer();

	glm::vec2 size = Window.GetViewport();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer.mHandle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFB.GetRenderBuffer());
	glBlitFramebuffer(0, 0, static_cast<GLint>(size.x), static_cast<GLint>(size.y), 0, 0, 
					static_cast<GLint>(size.x), static_cast<GLint>(size.y), GL_DEPTH_BUFFER_BIT, GL_NEAREST );
	glBindFramebuffer(GL_FRAMEBUFFER, mFB.GetRenderBuffer());

	//get shader program
	ShaderProgram& shader = mShaders[static_cast<size_t>(RenderMode::Lighting)];
	shader.Use();
	BindLights(shader);
	//Diabling the back face culling
	glDisable(GL_CULL_FACE);
	//SET BLENDING TO ADDITIVE
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_GREATER);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	for (auto& it : mLights)
	{
		//binding the screen triangle
		it.mModel->BindVAO();
		//set uniforms in shader
		glm::mat4x4 mv = Camera.GetCameraMat() * it.mM2W;
		glm::mat4x4 mvp = Camera.GetProjection() * mv;
		shader.SetMatUniform("MV", &mv[0][0]);
		shader.SetMatUniform("MVP", &mvp[0][0]);
		shader.SetVec2Uniform("Size", Window.GetViewport());

		//rendering the screen triangle
		const tinygltf::Scene& scene = it.mModel->GetGLTFModel().scenes[it.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*it.mModel, it.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
	}

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
}

void RenderManagerClass::BindLights(ShaderProgram& shader)
{
	//setting the number of light sources
	shader.SetIntUniform("LightNum", (int)mLights.size());

	//for each light set the uniforms
	for (int i = 0; i < mLights.size(); i++)
		mLights[i].SetUniforms("mLights[" + std::to_string(i) + "]", &shader);

	//binding the gbuffer textures as inputs
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mGBuffer.mDiffuseBuffer);
	glUniform1i(0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mGBuffer.mNormalBuffer);
	glUniform1i(1, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mGBuffer.mPositionBuffer);
	glUniform1i(2, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mGBuffer.mSpecularBuffer);
	glUniform1i(3, 3);

}

void RenderManagerClass::AmbientStage()
{
	//get shader program
	ShaderProgram& shader = mShaders[static_cast<size_t>(RenderMode::Ambient)];
	shader.Use();
	//binding the screen triangle
	mScreenTriangle->BindVAO();
	//set uniforms in shader
	glm::mat4x4 mvp = glm::scale(glm::vec3(1.0F));
	shader.SetMatUniform("MVP", &mvp[0][0]);
	shader.SetFloatUniform("Ambient", mAmbient);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mGBuffer.mDiffuseBuffer);
	glUniform1i(0, 0);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mScreenTriangle->GetGLTFModel().scenes[mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size(); i++)
		RenderNode(*mScreenTriangle, mScreenTriangle->GetGLTFModel().nodes[scene.nodes[i]]);

	glDisable(GL_BLEND);
	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
}

void RenderManagerClass::Display()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//get shader program
	ShaderProgram& shader = mShaders[static_cast<size_t>(RenderMode::Regular)];
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
	case DisplayTex::Position:
		glBindTexture(GL_TEXTURE_2D, mGBuffer.mPositionBuffer);
		break;
	case DisplayTex::Specular:
		glBindTexture(GL_TEXTURE_2D, mGBuffer.mSpecularBuffer);
		break;
	//case DisplayTex::Depth:
	//	glBindTexture(GL_TEXTURE_2D, mGBuffer.mDepth);
	//	break;
	default:
		glBindTexture(GL_TEXTURE_2D, mFB.GetRenderTexture());
		break;
	}

	glUniform1i(0, 0);

	//rendering the screen triangle
	const tinygltf::Scene& scene = mScreenTriangle->GetGLTFModel().scenes[mScreenTriangle->GetGLTFModel().defaultScene];
	for (size_t i = 0; i < scene.nodes.size(); i++)
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

ShaderProgram& RenderManagerClass::GetShader(const RenderMode& mode) { return mShaders[static_cast<size_t>(mode)]; }

GLuint RenderManagerClass::GenTexture(const glm::ivec2& size, bool high_precision)
{
	auto precision = high_precision ? GL_RGBA16F : GL_RGBA;

	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, precision, size.x, size.y, 0, GL_RGBA, high_precision ? GL_FLOAT : GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return handle;
}
