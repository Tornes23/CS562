#pragma once
#include <glm/vec3.hpp>

class Texture;
struct Decal
{
	Texture* mDiffuse;
	Texture* mNormal;
	Texture* mMetallic;

	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};