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
	const float  GetContrast() const;

	void  SetContrast(float val);

	void UseRenderBuffer();

	void ClearColorBuffer();
	void ClearDepthBuffer();


private:
	float mContrast;

	GLuint mRenderTexture;
	GLuint mRenderBuffer;
	GLuint mLuminenceTexture;

};