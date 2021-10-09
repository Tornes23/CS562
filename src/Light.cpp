#include <glm/gtx/transform.hpp>
#include "Light.h"
#include "Camera.h"
#include "Shader.h"

glm::mat4x4 Light::GetM2W() const
{
	glm::mat4x4 m2w(1.0);
	m2w = m2w * glm::translate(mPos);
	m2w = m2w * glm::scale(glm::vec3(mRadius * 2));
    return m2w;
}

void Light::SetUniforms(const std::string& name, ShaderProgram* shader)
{
	shader->SetVec3Uniform(name + ".PosInCamSpc", Camera.GetCameraMat() * glm::vec4(mPos, 1.0));
	shader->SetVec3Uniform(name + ".Attenuation", mAttenuation);
	shader->SetColorUniform(name + ".Color", mColor);
	shader->SetFloatUniform(name + ".Radius", mRadius);
}
