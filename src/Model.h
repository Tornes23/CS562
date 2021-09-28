#pragma once
#include "Mesh.h"

class Model
{
public:
	Model(const std::string& file);
	~Model();

private:

	std::vector<std::shared_ptr<Mesh>> mMeshes;
};
