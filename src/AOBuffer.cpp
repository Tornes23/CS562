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
	mAOTexture = RenderManager.GenTexture(size);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mAOTexture, 0);
	GLuint attachements[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachements);

	// Revert to the default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

const GLuint AOBuffer::GetRenderBuffer() const { return mRenderBuffer; }

const GLuint AOBuffer::GetAOTexture() const { return mAOTexture; }

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