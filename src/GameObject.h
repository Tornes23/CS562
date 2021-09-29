#pragma once
#include <string>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Model.h"

struct GameObject
{
	void Update();

	glm::mat4x4 mM2W;
	std::string mMesh;
	glm::vec3 mPos;
	glm::vec3 mRotation;
	glm::vec3 mScale;

	Model* mModel;
};

