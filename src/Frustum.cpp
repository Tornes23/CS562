#include <glm/gtx/transform.hpp>
#include "RenderManager.h"
#include "Frustum.h"

void Frustum::ComputeFrustum(float fov, float near, float far, const glm::vec3& pos, const glm::vec3& view, float ratio)
{
	glm::vec3 centre_near = pos + (glm::normalize(view) * near);
	glm::vec3 centre_far = pos + (glm::normalize(view) * far);

	float fov_radians = glm::radians(fov / 2.0F);
	float near_height = 2.0F * glm::tan(fov) * near;
	float near_width =  near_height * ratio;
	float far_height = 2.0F * glm::tan(fov) * far;
	float far_width = far_height * ratio;

	//near plane points (botLeft, botRight, topRight, topLeft)
	mPoints[0] = {-near_width, -near_height, 1};
	mPoints[1] = {near_width,  -near_height, 1};
	mPoints[2] = {near_width,   near_height, 1};
	mPoints[3] = {-near_width,  near_height, 1};

	//far plane points (botLeft, botRight, topRight, topLeft)
	mPoints[4] = {-far_width, -far_height, 1};
	mPoints[5] = {far_width,  -far_height, 1};
	mPoints[6] = {far_width,   far_height, 1};
	mPoints[7] = {-far_width,  far_height, 1};

}

void Frustum::GenShadowMap(const glm::vec2& size)
{
	mShadowMap.Create(true, size);
}

void Frustum::BindShadowBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMap.GetDepthBuffer());
	//clearing the buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//front face removal
	glCullFace(GL_FRONT);
}

std::vector<glm::vec4> Frustum::GetAABB(const glm::mat4x4& light)
{
	std::vector<glm::vec4> points_in_light;

	for (size_t i = 0; i < mPoints.size(); i++)
	{
		points_in_light.push_back(light * glm::vec4(mPoints[i], 1.0F));
	}
	return points_in_light;
}
