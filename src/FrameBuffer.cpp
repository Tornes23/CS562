#include "FrameBuffer.h"

void FrameBuffer::Create(int width, int height)
{
	//setting the variables
	mWidth = width;
	mHeight = height;

	mShadowMapWidth = 1024;
	mShadowMapHeight = 1024;

	mContrast = 1.0F;

	GenRenderBuffer();
	GenDepthBuffer();
}

void FrameBuffer::GenRenderBuffer()
{
	// Create to render texture (use window resolution)
	glGenTextures(1, &mRenderTexture);
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Expansion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create and set up the FBO
	glGenFramebuffers(1, &mRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	/* Write Your Code */

	// Set output color texture at attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture, 0);
	GLenum attachements[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachements);

	// Add depth render buffer to the frame buffer
	GLuint depthRenderBuffer;
	//generating
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	//setting it will store depth
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
	//attaching
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	// Revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FrameBuffer::GenDepthBuffer()
{
	// Create to render texture (use shadowmap resolution)
	glGenTextures(1, &mShadowMap);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowMapWidth, mShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Expansion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create and set up the FBO
	glGenFramebuffers(1, &mDepthBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthBuffer);

	// Set output color texture at attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

	GLenum attachements[] = { GL_NONE };

	glDrawBuffers(1, attachements);

	// Revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const GLuint FrameBuffer::GetRenderBuffer() const {	return mRenderBuffer; }

const GLuint FrameBuffer::GetRenderTexture() const {	return mRenderTexture; }

const float FrameBuffer::GetContrast() const { return mContrast; }

void FrameBuffer::SetContrast(float val){ mContrast = val; }

const GLuint FrameBuffer::GetDepthBuffer() const { return mDepthBuffer; }

void FrameBuffer::UseRenderBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	//clearing the buffer
	ClearColorBuffer();
	ClearDepthBuffer();
}

void FrameBuffer::UseDepthBuffer()
{
	//setting the size of the viewport
	glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);

	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthBuffer);

	//clearing the buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//back face removal
	glCullFace(GL_FRONT);
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