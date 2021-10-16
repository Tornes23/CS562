#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <gl/glew.h>
#undef near
#undef far

struct Frustum
{
	void ComputeFrustum(float fov, float near, float far, const glm::vec3& pos, const glm::vec3& view, float ratio);
	void BindBuffer();
	void CreateModel();
	GLsizei GetDrawElements();
	std::array<glm::vec3, 8> mPoints;
	std::array<unsigned short, 26> mIndices;
	bool mbFrusta;
	GLuint mVAO;
	GLuint mVBO[2];
};