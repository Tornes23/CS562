#pragma once
#include <glm/vec2.hpp>
#include <glew.h>

class GBuffer
{
public:
	void Create();
	void Bind();

private:
	GLuint mHandle;
	
	GLuint mNormalBuffer;
	GLuint mDiffuseBuffer;
	GLuint mPositionBuffer;
	GLuint mDepth;

	glm::ivec2 mSize;
};