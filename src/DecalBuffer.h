#pragma once
#include <glm/vec2.hpp>
#include <glew.h>

struct DecalBuffer
{
	void Create(GLuint diffuse, GLuint normal, GLuint specular);
	void Bind();
	void BindTextures();
	void BindDiffuseTexture();
	void BindNormalTexture();
	void BindSpecularTexture();

	void UseRenderBuffer();
	void BindDrawBuffer();
	GLuint mHandle;

	GLuint mDiffuseBuffer;
	GLuint mNormalBuffer;
	GLuint mSpecularBuffer;
	glm::ivec2 mSize;
};
