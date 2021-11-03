#pragma once
#include "gl/glew.h"

class BloomBuffer
{
public:

	void Create();
	void GenRenderBuffer();

	const GLuint GetRenderBuffer() const;
	const GLuint GetLuminenceTexture(bool first = true) const;

	void UseRenderBuffer();
	void BindDrawBuffer();


private:

	GLuint mRenderBuffer;
	GLuint mLuminenceTextures[2];

};