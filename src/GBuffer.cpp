#include <iostream>
#include "GBuffer.h"
#include "Window.h"
#include "RenderManager.h"

void GBuffer::Create()
{
	mSize = Window.GetViewport();

	glGenFramebuffers(1, &mHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

	// Attach the different textures of the GBuffer
	mPositionBuffer = RenderManager.GenTexture(mSize, true);
	mNormalBuffer   = RenderManager.GenTexture(mSize, true);
	mDiffuseBuffer  = RenderManager.GenTexture(mSize);
	mSpecularBuffer  = RenderManager.GenTexture(mSize);

	glGenTextures(1, &mDepth);
	glBindTexture(GL_TEXTURE_2D, mDepth);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mSize.x, mSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// Filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Expansion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDiffuseBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mPositionBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mSpecularBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0);

	GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_NONE };
	glDrawBuffers(5, attachments);

	// unbind 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, mHandle); }

