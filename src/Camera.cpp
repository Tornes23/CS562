#include "Utils.h"
#include "InputManager.h"
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
	mRightVector = glm::normalize(glm::cross({0,1,0}, mView));
	mSpeed = 2.0F;

}

void CameraClass::Move()
{
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

	//pitch-rotate around right
	if (KeyDown(Key::Up))
		mRotation.x += 0.1F;
	if (KeyDown(Key::Down))
		mRotation.x += 0.1F;

	//yaw rotation - rotate around (0,1,0)
	if (KeyDown(Key::Left))
		mRotation.x += 0.1F;
	if (KeyDown(Key::Right))
		mRotation.x += 0.1F;
}

void CameraClass::Update()
{
	Move();

	//recomputing the matrices

}
