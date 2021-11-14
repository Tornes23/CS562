#pragma once
#include "gl/glew.h"

class AOBuffer
{
public:

	void Create();
	void GenRenderBuffer();

	const GLuint GetRenderBuffer() const;
	const GLuint GetAOTexture() const;

	void UseRenderBuffer();
	void BindDrawBuffer();


private:

	GLuint mRenderBuffer;
	GLuint mAOTexture;

};