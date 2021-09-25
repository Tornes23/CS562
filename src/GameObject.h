#pragma once
#include <string>
#include "glm/vec3.hpp"

class GameObject
{
public:
	GameObject(const std::string& mesh, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca);

private:

	std::string mMesh;
	glm::vec3 mPos;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};

