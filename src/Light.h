#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include "Color.h"
class Model;
class ShaderProgram;

struct  Light
{
	enum class LightType
	{
		Point,
		Directional
	};
	void Update();
	void SetUniforms(const std::string& name, ShaderProgram* shader);
	glm::mat4x4 GetLightMatrix();

	glm::vec3 mPos{0.0F};
	glm::vec3 mDirection{ 0.0F };
	Color mColor = Color::Black;
	float mRadius = 0.0F;
	Model* mModel = nullptr;
	glm::mat4x4 mM2W{1.0F};
	bool mMoveUp = false;
	float mInitialY = 0.0F;
	LightType mType = LightType::Point;

};

