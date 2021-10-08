#pragma once
#include <glm/vec2.hpp>
#include <glew.h>

struct GBuffer
{
	void Create();
	void Bind();

	GLuint mHandle;
	
	GLuint mNormalBuffer;
	GLuint mDiffuseBuffer;
	GLuint mPositionBuffer;
	GLuint mDepth;

	glm::ivec2 mSize;
};