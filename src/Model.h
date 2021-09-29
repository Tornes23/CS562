#pragma once
#include <map>
#include "gl/glew.h"
#include "tinyglft/tiny_gltf.h"

class Model
{
public:
	Model(tinygltf::Model* model = nullptr);
	~Model();
	void BindSceneNodes(tinygltf::Model* model, tinygltf::Node& node);
	void BindMeshes(tinygltf::Model* model, tinygltf::Mesh& mesh);

private:
	GLuint mVAO;
	std::map<int, GLuint> mVBOs;
};
