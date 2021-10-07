#include "RenderManager.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "JSON.h"
#include "Utils.h"
#include "Camera.h"
#include "InputManager.h"

void RenderManagerClass::Initialize()
{
	mGBuffer.Create();
	LoadShaders();
}

void RenderManagerClass::Update()
{
	if (KeyDown(Key::F5))
		LoadShaders(true);
		
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
}

void RenderManagerClass::LoadShaders(bool reload)
{
	if (reload)
		FreeShaders();

	mShaders.push_back(ShaderProgram("./data/shaders/GeometryStg.vert", "./data/shaders/GeometryStg.frag"));
	mShaders.push_back(ShaderProgram("./data/shaders/LightingStg.vert", "./data/shaders/LightingStg.frag"));
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ClearBuffer();

	GeometryStage();
	LightingStage();
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
		glm::mat4x4 m2w_normal = glm::transpose(glm::inverse(mv));
		shader.SetMatUniform("MVP", &mvp[0][0]);
		shader.SetMatUniform("MV", &mv[0][0]);
		shader.SetMatUniform("m2w_normal", &m2w_normal[0][0]);

		const tinygltf::Scene& scene = it.mModel->GetGLTFModel().scenes[it.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*it.mModel, it.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
		
		//unbinding the VAO
		glBindVertexArray(0);
	}

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManagerClass::LightingStage()
{
	//get shader program
	ShaderProgram& shader = mShaders[static_cast<size_t>(RenderMode::Lighting)];
	shader.Use();
	for (auto& it : mLights)
	{
		it.mModel->BindVAO();
		//set uniforms in shader
		glm::mat4x4 mv = Camera.GetCameraMat() * it.GetM2W();
		glm::mat4x4 mvp = Camera.GetProjection() * mv;
		glm::mat4x4 m2w_normal = glm::transpose(glm::inverse(mv));
		shader.SetMatUniform("MVP", &mvp[0][0]);
		shader.SetMatUniform("MV", &mv[0][0]);
		shader.SetMatUniform("m2w_normal", &m2w_normal[0][0]);
	
		const tinygltf::Scene& scene = it.mModel->GetGLTFModel().scenes[it.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*it.mModel, it.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
	}
	
	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);
}

void RenderManagerClass::Display()
{
	//TEMPORALLY RENDER SCENE REGULAR WAY
	auto objs = GOManager.GetObjs();
	//get shader program
	ShaderProgram& shader = mShaders[static_cast<size_t>(RenderMode::Regular)];
	shader.Use();
	for (auto& it : objs)
	{
		it.mModel->BindVAO();
		//set uniforms in shader
		glm::mat4x4 mv = Camera.GetCameraMat() * it.mM2W;
		glm::mat4x4 mvp = Camera.GetProjection() * mv;
		glm::mat4x4 m2w_normal = glm::transpose(glm::inverse(mv));
		shader.SetMatUniform("MVP", &mvp[0][0]);
		shader.SetMatUniform("MV", &mv[0][0]);
		shader.SetMatUniform("m2w_normal", &m2w_normal[0][0]);

		const tinygltf::Scene& scene = it.mModel->GetGLTFModel().scenes[it.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
			RenderNode(*it.mModel, it.mModel->GetGLTFModel().nodes[scene.nodes[i]]);
	}

	glUseProgram(0);
	//unbinding the VAOs
	glBindVertexArray(0);

	//render screentriangle
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

		glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType,
						BUFFER_OFFSET(indexAccessor.byteOffset));
	}
}

ShaderProgram& RenderManagerClass::GetShader() { return mShaders[static_cast<size_t>(mMode)]; }

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
