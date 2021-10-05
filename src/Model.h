#pragma once
#include <map>
#include "gl/glew.h"
#include "tinyglft/tiny_gltf.h"
#include "Material.h"

class Model
{
public:
	Model(tinygltf::Model* model);
	~Model();
	void BindSceneNodes(tinygltf::Model* model, tinygltf::Node& node);
	void BindMeshes(tinygltf::Model* model, tinygltf::Mesh& mesh);
	tinygltf::Model& GetGLTFModel();
	void BindVAO();
	void SetMaterialActive(int index);
	void GetMaterials();

private:
	GLuint mVAO;
	std::map<int, GLuint> mVBOs;
	tinygltf::Model& mGLTF_Model;
	std::vector<Material*> mMaterials;
};
