#pragma once

#include "Window.h"
#include "Texture.h"
#include "Mesh.h"

class FrameBuffer
{
private:
	int width, height;
	GLuint FBO, RBO;
	GLuint texColor, texDepth;
	std::shared_ptr<Texture> colorTexture;
	std::shared_ptr<Texture> depthTexture;

	void init(int w, int h, bool useDepth, bool useMs, bool useDepthMap);

public:
	FrameBuffer(Window& window, bool useDepth = true, bool useMs = false, bool useDepthMap = false);

	FrameBuffer(int w = 1920, int h = 1080, bool useDepth = true, bool useMs = false, bool useDepthMap = false);

	~FrameBuffer();

	GLuint getFBO() const { return FBO; }

	std::shared_ptr<Texture> getColorTexture() const { return colorTexture; }
	std::shared_ptr<Texture> getDepthTexture() const { return depthTexture; }
};

