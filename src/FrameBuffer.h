#pragma once
#include <gl/glew.h>
#include <glm/vec2.hpp>

class FrameBuffer
{
public:

	void Create(bool depth_only = false, const glm::vec2& size = glm::vec2(0, 0));
	void GenRenderBuffer();
	void GenDepthBuffer(const glm::vec2& size);

	const GLuint GetRenderBuffer() const;
	const GLuint GetDepthBuffer() const;
	const GLuint GetRenderTexture() const;
	const GLuint GetLuminenceTexture() const;
	const GLuint GetTexture(bool first) const;

	void UseRenderBuffer();

	void ClearColorBuffer();
	void ClearDepthBuffer();


private:

	GLuint mRenderTexture;
	GLuint mRenderBuffer;
	GLuint mLuminenceTexture;
	GLuint mDepthTex;
	GLuint mDepthBuffer;
	bool mDepthOnly;

};