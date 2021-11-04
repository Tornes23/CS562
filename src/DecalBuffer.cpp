#include <iostream>
#include "DecalBuffer.h"
#include "Window.h"
#include "RenderManager.h"

void DecalBuffer::Create()
{
	mSize = Window.GetViewport();

	glGenFramebuffers(1, &mHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

	// Attach the different textures of the DecalBuffer
	mNormalBuffer = RenderManager.GenTexture(mSize, true);
	mDiffuseBuffer = RenderManager.GenTexture(mSize);
	mSpecularBuffer = RenderManager.GenTexture(mSize);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDiffuseBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mSpecularBuffer, 0);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// unbind 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DecalBuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, mHandle); }

void DecalBuffer::BindTextures()
{
	//binding the gbuffer textures as inputs
	BindDiffuseTexture();
	BindNormalTexture();
	BindSpecularTexture();

}

void DecalBuffer::BindDiffuseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDiffuseBuffer);
	glUniform1i(0, 0);
}

void DecalBuffer::BindNormalTexture()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mNormalBuffer);
	glUniform1i(1, 1);
}


void DecalBuffer::BindSpecularTexture()
{
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mSpecularBuffer);
	glUniform1i(3, 3);
}

void DecalBuffer::UseRenderBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
}

void DecalBuffer::BindDrawBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mHandle);
}
