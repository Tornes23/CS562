#include <glm/gtx/transform.hpp>
#include "RenderManager.h"
#include "Light.h"
#include "Camera.h"
#include "Shader.h"

void Light::Update()
{
	if (RenderManager.LightsAnimated())
	{
		if (mMoveUp)
		{
			mPos.y += 0.1F;
			if (mPos.y > (mInitialY + 10.0F))
				mMoveUp = false;
		}
		else
		{
			mPos.y -= 0.1F;
			if (mPos.y < (mInitialY - 10.0F))
				mMoveUp = true;
		}
	}
	glm::mat4x4 m2w(1.0);
	m2w = m2w * glm::translate(mPos);
	m2w = m2w * glm::scale(glm::vec3(2.0F * mRadius));
	mM2W = m2w;
}

void Light::SetUniforms(const std::string& name, ShaderProgram* shader)
{
	glm::vec3 posInCam = Camera.GetCameraMat() * glm::vec4(mPos, 1.0F);
	shader->SetVec3Uniform(name + ".PosInCamSpc", posInCam);
	shader->SetColorUniform(name + ".Color", mColor);
	shader->SetFloatUniform(name + ".Radius", mRadius);
}
