#include "FrameBuffer.h"
#include "Window.h"
#include "RenderManager.h"

void FrameBuffer::Create()
{
	GenRenderBuffer();
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

	// Add depth render buffer to the frame buffer
	GLuint depthRenderBuffer;
	//generating
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	//setting it will store depth
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
	//attaching
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	// Revert to the default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

const GLuint FrameBuffer::GetRenderBuffer() const {	return mRenderBuffer; }

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