#pragma once
#include "gl/glew.h"

class FrameBuffer
{
public:

	void Create();
	void GenRenderBuffer();

	const GLuint GetRenderBuffer() const;
	const GLuint GetRenderTexture() const;

	void UseRenderBuffer();
	void BindDrawBuffer();


private:

	GLuint mRenderTexture;
	GLuint mRenderBuffer;
};