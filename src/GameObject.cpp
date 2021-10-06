#include "GameObject.h"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"

void GameObject::Update()
{
	//updating the model to world matrix
	mM2W = glm::mat4x4(1.0);
	mM2W = glm::translate(mM2W, mPos);
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.x), glm::vec3(1, 0, 0));
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.y), glm::vec3(0, 1, 0));
	mM2W = mM2W * glm::rotate(glm::radians(mRotation.z), glm::vec3(0, 0, 1));
	mM2W = glm::scale(mScale);
}
