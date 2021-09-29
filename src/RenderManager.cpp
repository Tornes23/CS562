#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Utils.h"
#include "Camera.h"

void RenderManagerClass::Initialize()
{
	LoadShaders();
}

void RenderManagerClass::LoadLights(const nlohmann::json& lights)
{
	for (auto it = lights.begin(); it != lights.end(); it++)
	{
		Light light;
		nlohmann::json object = *it;
		//load light
		object >> light;
		//load mesh
		mLights.push_back(light);
	}
}

void RenderManagerClass::LoadShaders()
{
	mShaders.push_back(ShaderProgram("./data/shaders/regular.vert", "./data/shaders/regular.frag"));
}

void RenderManagerClass::Render()
{
	auto objs = GOManager.GetObjs();
	for (auto& it : objs)
	{
		//get shader program
		mShaders[0].Use();
		//set uniforms in shader
		glm::mat4x4 mvp = Camera.GetProjection()* Camera.GetCameraMat() * it.mM2W;
		mShaders[0].SetMatUniform("MVP", &mvp[0][0]);

		it.mModel->BindVAO();

		const tinygltf::Scene& scene = it.mModel->GetGLTFModel().scenes[it.mModel->GetGLTFModel().defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); ++i) 
			RenderNode(it.mModel->GetGLTFModel(), it.mModel->GetGLTFModel().nodes[scene.nodes[i]]);

	}

	//unbinding the VAOs
	glBindVertexArray(0);
	glUseProgram(0);
}

void RenderManagerClass::RenderNode(const tinygltf::Model& model, const tinygltf::Node& node)
{
	if((node.mesh >= 0) && (node.mesh < model.meshes.size())) 
		RenderMesh(model, model.meshes[node.mesh]);

	for (size_t i = 0; i < node.children.size(); i++) 
		RenderNode(model, model.nodes[node.children[i]]);
}

void RenderManagerClass::RenderMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
{
	for (size_t i = 0; i < mesh.primitives.size(); ++i) {
		tinygltf::Primitive primitive = mesh.primitives[i];
		tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

		glDrawElements(primitive.mode, indexAccessor.count,
			indexAccessor.componentType,
			BUFFER_OFFSET(indexAccessor.byteOffset));
	}
}
