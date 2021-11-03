#pragma once
#include <glm/vec2.hpp>
#include <glew.h>

struct DecalBuffer
{
	void Create();
	void Bind();
	void BindTextures();
	void BindDiffuseTexture();
	void BindNormalTexture();
	void BindSpecularTexture();
	GLuint mHandle;

	GLuint mDiffuseBuffer;
	GLuint mNormalBuffer;
	GLuint mSpecularBuffer;
	glm::ivec2 mSize;
};
