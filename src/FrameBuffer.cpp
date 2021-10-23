#include <iostream>
#include "FrameBuffer.h"
#include "Window.h"
#include "RenderManager.h"

void FrameBuffer::Create(bool depth_only, const glm::vec2& size)
{
	mDepthOnly = depth_only;

	if(!depth_only)
		GenRenderBuffer();

	GenDepthBuffer(size);
}

void FrameBuffer::GenDepthBuffer(const glm::vec2& size)
{
	mDepthTex = RenderManager.GenTexture(size, false, true);

	// Create and set up the FBO
	glGenFramebuffers(1, &mDepthBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthBuffer);
	// Set output color texture at attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTex, 0);
	GLenum attachements[] = { GL_NONE };
	glDrawBuffers(1, attachements);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		std::cerr << "Error creating depth buffer\n";
	}

	// Revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FrameBuffer::GenRenderBuffer()
{
	// Create and set up the FBO
	glGenFramebuffers(1, &mRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	// Create to render texture (use window resolution)
	glm::ivec2 size = Window.GetViewport();
	mRenderTexture = RenderManager.GenTexture(size, true);
	mLuminenceTexture = RenderManager.GenTexture(size, true);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mLuminenceTexture, 0);
	GLuint attachements[2] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachements);
}

const GLuint FrameBuffer::GetRenderBuffer() const {	return mRenderBuffer; }

const GLuint FrameBuffer::GetDepthBuffer() const { return mDepthBuffer; }

const GLuint FrameBuffer::GetRenderTexture() const { return mRenderTexture; }

const GLuint FrameBuffer::GetLuminenceTexture() const { return mLuminenceTexture; }

const GLuint FrameBuffer::GetTexture(bool first) const { return first ? mRenderBuffer : mLuminenceTexture; }

void FrameBuffer::UseRenderBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);
}

void FrameBuffer::ClearColorBuffer()
{
	// Clear framebuffer
	glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::ClearDepthBuffer()
{
	// Clear framebuffer
	glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
}