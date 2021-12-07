#pragma once
#include "gl/glew.h"

class RTXBuffer
{
public:

	void Create();
	void GenRenderBuffer();

	const GLuint GetRenderBuffer() const;
	const GLuint GetRTXTexture() const;

	void UseRenderBuffer();
	void BindDrawBuffer();


private:

	GLuint mRenderBuffer;
	GLuint mRTXTexture;

};