#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Texture;
class Model;

struct Decal
{	
	Texture* mDiffuse;
	Texture* mNormal;
	Texture* mMetallic;

	void GenM2W();
	void SetUniforms();
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
	glm::mat4x4 mM2W;

	Model* mModel;
};