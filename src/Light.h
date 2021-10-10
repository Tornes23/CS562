#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include "Color.h"
class Model;
class ShaderProgram;

struct  Light
{
	glm::mat4x4 GetM2W() const;
	glm::vec3 mPos;
	Color mColor;
	float mRadius;
	Model* mModel;
	void SetUniforms(const std::string& name, ShaderProgram* shader);

};

