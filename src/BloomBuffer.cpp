#include "BloomBuffer.h"
#include "Window.h"
#include "RenderManager.h"

void BloomBuffer::Create()
{
	GenRenderBuffer();
}

void BloomBuffer::GenRenderBuffer()
{
	// Create and set up the FBO
	glGenFramebuffers(1, &mRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	// Create to render texture (use window resolution)
	glm::ivec2 size = Window.GetViewport();
	mLuminenceTextures[0] = RenderManager.GenTexture(size, true);
	mLuminenceTextures[1] = RenderManager.GenTexture(size, true);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mLuminenceTextures[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mLuminenceTextures[1], 0);
	GLuint attachements[2] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachements);

	// Revert to the default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

const GLuint BloomBuffer::GetRenderBuffer() const { return mRenderBuffer; }

const GLuint BloomBuffer::GetLuminenceTexture(bool first) const { return first ? mLuminenceTextures[0] : mLuminenceTextures[1]; }

void BloomBuffer::UseRenderBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);
}

void BloomBuffer::BindDrawBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mRenderBuffer);
}