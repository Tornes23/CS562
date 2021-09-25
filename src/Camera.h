#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "json/json.hpp"

class CameraClass
{
public:
	//singleton stuff
	CameraClass(CameraClass const&) = delete;
	void operator=(CameraClass const&) = delete;
	static CameraClass& GetInstance()
	{
		static CameraClass instance;
		return instance;
	}

	void LoadCamera(const nlohmann::json& j);
	void Move();
	void Update();

private:
	glm::mat4x4 mPerspective;
	glm::mat4x4 mCamera;

	float mNear;
	float mFar;
	float mFOV;
	float mSpeed;

	glm::vec3 mView;//view vector
	glm::vec3 mRightVector;//right vector
	glm::vec3 mPos;
	glm::vec3 mRotation;

	CameraClass() { }
};

#define Camera (CameraClass::GetInstance())