#pragma once
#include <vector>
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <gl/glew.h>
#include "FrameBuffer.h"
#undef near
#undef far

struct Frustum
{
	void ComputeFrustum(float fov, float near, float far, const glm::vec3& pos, const glm::vec3& view, float ratio);
	void GenShadowMap(const glm::vec2& size);
	void BindShadowBuffer();
	std::vector<glm::vec4> GetAABB(const glm::mat4x4& light);
	std::array<glm::vec3, 8> mPoints;
	FrameBuffer mShadowMap;
	bool mbFrusta;
};