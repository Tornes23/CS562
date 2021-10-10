#pragma once
#include "gl/glew.h"

class FrameBuffer
{
public:

	void Create(int width = 1280, int height = 720);
	void GenRenderBuffer();
	void GenDepthBuffer();

	const GLuint GetRenderBuffer() const;
	const GLuint GetRenderTexture() const;
	const GLuint GetDepthBuffer() const;
	const float  GetContrast() const;

	void  SetContrast(float val);

	void UseRenderBuffer();
	void UseDepthBuffer();

	void ClearColorBuffer();
	void ClearDepthBuffer();


private:

	int mWidth;
	int mHeight;

	int mShadowMapWidth;
	int mShadowMapHeight;

	float mContrast;

	GLuint mShadowMap;

	GLuint mRenderTexture;
	GLuint mRenderBuffer;
	GLuint mDepthBuffer;

};