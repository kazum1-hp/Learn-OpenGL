#include "../head/FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(Window& window, bool useDepth)
	: width(window.getWidth()), height(window.getHeight())
{
	init(width, height, useDepth);
}

FrameBuffer::FrameBuffer(int w, int h, bool useDepth)
	: width(w), height(h)
{
	init(width, height, useDepth);
}

void FrameBuffer::init(int w, int h, bool useDepth)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// texture attachment
	glGenTextures(1, &texColor);
	glBindTexture(GL_TEXTURE_2D, texColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);

	colorTexture = std::make_shared<Texture>(texColor, 0);

	// renderbuffer attachment
	if (useDepth)
	{
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR :: FRAMEBUFFER :: FrameBuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteTextures(1, &texColor);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteFramebuffers(1, &FBO);
}