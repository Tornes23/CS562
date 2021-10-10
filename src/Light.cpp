#include <glm/gtx/transform.hpp>
#include "Light.h"
#include "Camera.h"
#include "Shader.h"

void Light::Update()
{
	if (mAnimated)
	{
		mSin += 0.01F;

		if (mSin >= 360.0F)
			mSin = 0.0F;

		mPos.y = (sinf(mSin) * 20);
	}

	glm::mat4x4 m2w(1.0);
	m2w = m2w * glm::translate(mPos);
	m2w = m2w * glm::scale(glm::vec3(2.0F * mRadius));
	mM2W = m2w;
}

void Light::SetUniforms(const std::string& name, ShaderProgram* shader)
{
	glm::vec3 posInCam = Camera.GetCameraMat() * (mM2W * glm::vec4(mPos, 1.0F));
	shader->SetVec3Uniform(name + ".PosInCamSpc", posInCam);
	shader->SetColorUniform(name + ".Color", mColor);
	shader->SetFloatUniform(name + ".Radius", mRadius);
}
