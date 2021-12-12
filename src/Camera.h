#pragma once
#include "json/json.hpp"
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
	void Save(nlohmann::json& j);
	void UpdateVectors(const glm::vec2& offset);
	void Rotate();
	glm::mat4x4 GetProjection() const;
	glm::mat4x4 GetCameraMat() const;
	glm::vec3 GetViewVec() const;
	float GetNear() const;
	float GetFar() const;

private:
	glm::mat4x4 mPerspective;
	glm::mat4x4 mCameraMat;

	float mNear;
	float mFar;
	float mFOV;
	float mSpeed;
	float mSensitivity;

	glm::vec3 mView;//view vector
	glm::vec3 mUp;//up vector
	glm::vec3 mRightVector;//right vector
	glm::vec3 mPos;
	glm::vec3 mRotation;

	glm::ivec2 mPrevMousePos;

	CameraClass() { }
};

#define Camera (CameraClass::GetInstance())