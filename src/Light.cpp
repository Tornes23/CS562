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

	mM2W = glm::translate(mPos);
	mM2W = mM2W * glm::scale(glm::vec3(2.0F * mRadius));
}

void Light::SetUniforms(const std::string& name, ShaderProgram* shader)
{
	glm::vec3 posInCam = Camera.GetCameraMat() * glm::vec4(mPos, 1.0F);
	shader->SetVec3Uniform(name + ".PosInCamSpc", posInCam);
	shader->SetColorUniform(name + ".Color", mColor);
	shader->SetFloatUniform(name + ".Radius", mRadius);
}
