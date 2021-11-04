#pragma once
#include <glm/vec2.hpp>
#include <glew.h>

struct GBuffer
{
	void Create();
	void Bind();
	void BindTextures();
	void BindDiffuseTexture();
	void BindNormalTexture();
	void BindSpecularTexture();
	void BindDepthTexture();
	void BindReadBuffer();
	GLuint mHandle;
	
	GLuint mNormalBuffer;
	GLuint mDiffuseBuffer;
	GLuint mSpecularBuffer;
	GLuint mDepth;

	glm::ivec2 mSize;
};