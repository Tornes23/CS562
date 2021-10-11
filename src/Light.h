#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include "Color.h"
class Model;
class ShaderProgram;

struct  Light
{
	void Update();
	glm::vec3 mPos;
	Color mColor;
	float mRadius;
	Model* mModel;
	glm::mat4x4 mM2W;
	void SetUniforms(const std::string& name, ShaderProgram* shader);
	bool mMoveUp = false;
	float mInitialY;

};

