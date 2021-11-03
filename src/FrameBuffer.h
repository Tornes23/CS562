#pragma once
#include "gl/glew.h"

class FrameBuffer
{
public:

	void Create();
	void GenRenderBuffer();

	const GLuint GetRenderBuffer() const;
	const GLuint GetRenderTexture() const;
	const GLuint GetLuminenceTexture() const;
	const GLuint GetTexture(bool first) const;

	void UseRenderBuffer();
	void BindDrawBuffer();

	void ClearColorBuffer();
	void ClearDepthBuffer();


private:

	GLuint mRenderTexture;
	GLuint mRenderBuffer;
	GLuint mLuminenceTexture;

};