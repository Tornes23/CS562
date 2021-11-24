#include "AOBuffer.h"
#include "Window.h"
#include "RenderManager.h"

void AOBuffer::Create()
{
	GenRenderBuffer();
}

void AOBuffer::GenRenderBuffer()
{
	// Create and set up the FBO
	glGenFramebuffers(1, &mRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	// Create to render texture (use window resolution)
	glm::ivec2 size = Window.GetViewport();
	mAOTexture[0] = RenderManager.GenTexture(size, true);
	mAOTexture[1] = RenderManager.GenTexture(size, true);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mAOTexture[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mAOTexture[1], 0);
	GLuint attachements[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachements);

	// Revert to the default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

const GLuint AOBuffer::GetRenderBuffer() const { return mRenderBuffer; }

const GLuint AOBuffer::GetAOTexture(bool first) const { return first? mAOTexture[0] : mAOTexture[1]; }

void AOBuffer::UseRenderBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);
}

void AOBuffer::BindDrawBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mRenderBuffer);
}