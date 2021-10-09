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
	mPositionBuffer = RenderManager.GenTexture(mSize);
	mNormalBuffer   = RenderManager.GenTexture(mSize);
	mDiffuseBuffer  = RenderManager.GenTexture(mSize);
	mSpecularBuffer  = RenderManager.GenTexture(mSize);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDiffuseBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mPositionBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mSpecularBuffer, 0);

	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	// Attach the depth buffer
	glGenRenderbuffers(1, &mDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mSize.x, mSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepth);
	
	// sanity check that everything went fine
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Unable to generate framebuffer" << std::endl;
		return;
	}

	// unbind 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, mHandle); }

