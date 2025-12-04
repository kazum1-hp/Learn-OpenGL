#include "../head/FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(Window& window, bool useDepth, bool useMs, bool useDepthMap2D, bool useDepthCube)
	: width(window.getWidth()), height(window.getHeight())
{
	init(width, height, useDepth, useMs, useDepthMap2D, useDepthCube);
}

FrameBuffer::FrameBuffer(unsigned int w, unsigned int h, bool useDepth, bool useMs, bool useDepthMap2D, bool useDepthCube)
	: width(w), height(h)
{
	init(width, height, useDepth, useMs, useDepthMap2D, useDepthCube);
}

void FrameBuffer::init(unsigned int w, unsigned int h, bool useDepth, bool useMs, bool useDepthMap2D, bool useDepthCube)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // color attachment (non-MSAA)
    if (!useMs && !useDepthMap2D && !useDepthCube) {
        glGenTextures(1, &texColor);
        glBindTexture(GL_TEXTURE_2D, texColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);

    }
    else if (useMs && !useDepthMap2D && !useDepthCube) {
        // multisample color texture attachment
        glGenTextures(1, &texColor);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texColor);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, w, h, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texColor, 0);
    }

    // depth buffer options:
    // 1) useDepthMap2D => create and attach a 2D depth texture (for directional shadow maps)
    // 2) useDepthCube  => create and attach a cube depth texture (for point light shadow maps)
    // 3) useDepth (renderbuffer) => create RBO depth/stencil

    if (useDepthMap2D) {
        // create 2D depth texture
        glGenTextures(1, &texDepth2D);
        glBindTexture(GL_TEXTURE_2D, texDepth2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepth2D, 0);

        // no color buffer needed in pure depth FBO usage
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else if (useDepthCube) {
        // create cube depth texture
        glGenTextures(1, &texDepthCube);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texDepthCube);
        for (unsigned int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // attach the whole cubemap as depth attachment (valid: will allow layered rendering with GS)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texDepthCube, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else if (useDepth) {
        // renderbuffer depth/stencil
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        if (useMs) {
            // multisample RBO handled elsewhere in code path; but keep here for completeness
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, w, h);
        }
        else {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        }
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR :: FRAMEBUFFER :: FrameBuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	if (texColor) glDeleteTextures(1, &texColor);
	if (texDepth2D) glDeleteTextures(1, &texDepth2D);
	if (texDepthCube) glDeleteTextures(1, &texDepthCube);
	if (RBO) glDeleteRenderbuffers(1, &RBO);
	if (FBO) glDeleteFramebuffers(1, &FBO);
}