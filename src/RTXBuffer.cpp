#include "RTXBuffer.h"
#include "Window.h"
#include "RenderManager.h"

void RTXBuffer::Create()
{
	GenRenderBuffer();
}

void RTXBuffer::GenRenderBuffer()
{
	// Create and set up the FBO
	glGenFramebuffers(1, &mRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	// Create to render texture (use window resolution)
	glm::ivec2 size = Window.GetViewport();
	mRTXTexture = RenderManager.GenTexture(size, true);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRTXTexture, 0);
	GLuint attachements[1] = { GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, attachements);

	// Revert to the default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

const GLuint RTXBuffer::GetRenderBuffer() const { return mRenderBuffer; }

const GLuint RTXBuffer::GetRTXTexture() const { return mRTXTexture; }

void RTXBuffer::UseRenderBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);
}

void RTXBuffer::BindDrawBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mRenderBuffer);
}