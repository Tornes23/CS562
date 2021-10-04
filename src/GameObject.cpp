#include "GameObject.h"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"

void GameObject::Update()
{
	//updating the model to world matrix
	mM2W = glm::mat4x4(1.0);
	mM2W = glm::translate(mM2W, mPos);
	mM2W = glm::scale(mScale);
}
