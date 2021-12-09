#pragma once
#include <map>
#include "gl/glew.h"
#include "tinyglft/tiny_gltf.h"
#include "Material.h"

class Model
{
public:
	Model(tinygltf::Model* model, const std::string& name);
	~Model();
	void BindSceneNodes(tinygltf::Model* model, tinygltf::Node& node);
	void BindMeshes(tinygltf::Model* model, tinygltf::Mesh& mesh);
	tinygltf::Model& GetGLTFModel();
	std::string GetName();
	void BindVAO();
	void SetMaterialActive(int index);
	void GetMaterials();
	std::vector<Material*>& GetMaterialVector();

	std::map<int, GLuint> mVBOs;
private:
	std::string mName;
	GLuint mVAO;
	tinygltf::Model& mGLTF_Model;
	std::vector<Material*> mMaterials;
};
