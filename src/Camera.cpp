#include "glm/gtx/transform.hpp"
#include "InputManager.h"
#include "Window.h"
#include "Utils.h"
#include "Camera.h"


void CameraClass::LoadCamera(const nlohmann::json& j)
{
	j["translate"] >> mPos;
	j["rotate"] >> mRotation;
	
	j["near"] >> mNear;
	j["far"] >> mFar;
	j["FOVy"] >> mFOV;

	//set view vector
	mView = { 1,0,0 };
	//set right vector
	mUp = {0,1,0};
	mRightVector = glm::normalize(glm::cross(mUp, mView));
	mSpeed = 2.0F;
	mSensitivity = 0.01F;
}

void CameraClass::Move()
{
	Rotate();

	//displacement
	if (KeyDown(Key::W))
		mPos += mSpeed * mView;
	if (KeyDown(Key::S))
		mPos -= mSpeed * mView;
	if (KeyDown(Key::D))
		mPos += mSpeed * mRightVector;
	if (KeyDown(Key::A))
		mPos -= mSpeed * mRightVector;
	if (KeyDown(Key::Q))
		mPos += mSpeed * glm::vec3(0,1,0);
	if (KeyDown(Key::E))
		mPos -= mSpeed * glm::vec3(0, 1, 0);
}

void CameraClass::Update()
{
	Move();
	//recomputing the matrices
	glm::ivec2 view = Window.GetViewport();
	mCameraMat = glm::lookAt(mPos, mPos + mView, mUp);
	mPerspective = glm::perspective(glm::radians(mFOV), static_cast<float>(view.x) / static_cast<float>(view.y), mNear, mFar);

}

void CameraClass::UpdateVectors(const glm::vec2& offset)
{
	glm::vec2 movement = offset * mSensitivity;
	mView = glm::vec3(glm::vec4(mView, 0) * glm::rotate(glm::radians(5.0f) * movement.y, mRightVector));
	mView = glm::vec3(glm::vec4(mView, 0) * glm::rotate(glm::radians(5.0f) * -movement.x, mUp));

	mUp = glm::vec3(glm::vec4(mUp, 0) * glm::rotate(glm::radians(5.0f) * movement.y, mRightVector));
	mRightVector = glm::normalize(glm::cross(mView, mUp));

}

void CameraClass::Rotate()
{
	if (MouseDown(MouseKey::RIGHT))
	{
		//computing the offset increment with the senitivity
		glm::vec2 offset = mPrevMousePos - InputManager.RawMousePos();
		UpdateVectors(offset);

	}
	mPrevMousePos = InputManager.RawMousePos();
}

glm::mat4x4 CameraClass::GetProjection() const { return mPerspective; }

glm::mat4x4 CameraClass::GetCameraMat() const { return mCameraMat;}
