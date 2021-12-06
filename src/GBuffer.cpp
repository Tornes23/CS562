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
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mSpecularBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0);

	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_NONE };
	glDrawBuffers(4, attachments);

	// unbind 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, mHandle); }

void GBuffer::BindTextures()
{
	//binding the gbuffer textures as inputs
	BindDiffuseTexture();
	BindNormalTexture();
	BindSpecularTexture();
	BindDepthTexture();

}

void GBuffer::BindDiffuseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDiffuseBuffer);
	glUniform1i(0, 0);
}

void GBuffer::BindNormalTexture()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mNormalBuffer);
	glUniform1i(1, 1);
}

void GBuffer::BindSpecularTexture()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mSpecularBuffer);
	glUniform1i(2, 2);
}

void GBuffer::BindDepthTexture()
{
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mDepth);
	glUniform1i(3, 3);
}

void GBuffer::BindReadBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mHandle);
}

