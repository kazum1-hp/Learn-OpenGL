#pragma once

#include "Window.h"
#include "Texture.h"
#include "Mesh.h"

class FrameBuffer
{
private:
	unsigned int width, height;
	GLuint FBO = 0, RBO = 0;
	GLuint texColor, texDepth2D, texDepthCube;
	bool m_useDepth;
	bool m_useMs;
	bool m_useDepthMap2D;
	bool m_useDepthCube;
	bool m_useHdr;

	void init(unsigned int w, unsigned int h);
	

public:
	FrameBuffer(Window& window, bool useDepth = true, bool useMs = false, bool useDepthMap2D = false, bool useDepthCube = false, bool useHdr = false);

	FrameBuffer(unsigned int w = 1920, unsigned int h = 1080, bool useDepth = true, bool useMs = false, bool useDepthMap2D = false, bool useDepthCube = false, bool useHdr = false);

	~FrameBuffer();

	void resize(unsigned int newWidth, unsigned int newHeight);

	GLuint getFBO() const { return FBO; }
	GLuint getColor() const { return texColor; }
	GLuint getDepth2D() const { return texDepth2D; }
	GLuint getDepthCube() const { return texDepthCube; }
};

