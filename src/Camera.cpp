#include "glm/gtx/transform.hpp"
#include "InputManager.h"
#include "Window.h"
#include "JSON.h"
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
	mRightVector = glm::normalize(glm::cross(mView, mUp));
	mSpeed = 2.0F;
	mSensitivity = 0.01F;
	//create frustum
	mFrustum.mbFrusta = false;
	mFrustum.ComputeFrustum(mFOV, mNear, mFar, mPos, mView, Window.GetAspectRatio());
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
	mView = glm::vec3(glm::vec4(mView, 0) * glm::rotate(glm::radians(5.0f) * -movement.x, glm::vec3(0,1,0)));

	mRightVector = glm::normalize(glm::cross(mView, mUp));

}

void CameraClass::Rotate()
{
	glm::vec2 offset(0.0F, 0.0F);
	if (KeyDown(Key::Down))
		offset = glm::vec2(0.0F, 10.0F);
	if (KeyDown(Key::Up))
		offset = glm::vec2(0.0F, -10.0F);
	if (KeyDown(Key::Left))
		offset = glm::vec2(10.0F,0.0F);
	if (KeyDown(Key::Right))
		offset = glm::vec2(-10.0F, 0.0F);

	if (MouseDown(MouseKey::RIGHT))
		offset = mPrevMousePos - InputManager.RawMousePos();

	UpdateVectors(offset);
	mPrevMousePos = InputManager.RawMousePos();
}

glm::mat4x4 CameraClass::GetProjection() const { return mPerspective; }

glm::mat4x4 CameraClass::GetCameraMat() const { return mCameraMat;}

float CameraClass::GetFOV() const { return mFOV; }
float CameraClass::GetNear() const { return mNear; }
float CameraClass::GetFar() const { return mFar; }
glm::vec3 CameraClass::GetPos() const { return mPos; }
glm::vec3 CameraClass::GetView() const { return mView; }
Frustum CameraClass::GetFrustum() const { return mFrustum; }
