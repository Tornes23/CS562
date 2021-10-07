#include <glm/gtx/transform.hpp>
#include "Light.h"

glm::mat4x4 Light::GetM2W() const
{
	glm::mat4x4 m2w(1.0);
	m2w = glm::translate(m2w, mPos);
	m2w = glm::scale(glm::vec3(mRadius));
    return m2w;
}
