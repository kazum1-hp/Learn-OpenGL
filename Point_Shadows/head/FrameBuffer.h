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
	std::shared_ptr<Texture> colorTexture;
	std::shared_ptr<Texture> depthTexture2D;
	std::shared_ptr<Texture> depthCubeTexture;

	void init(unsigned int w, unsigned int h, bool useDepth, bool useMs, bool useDepthMap2D, bool useDepthCube);

public:
	FrameBuffer(Window& window, bool useDepth = true, bool useMs = false, bool useDepthMap2D = false, bool useDepthCube = false);

	FrameBuffer(unsigned int w = 1920, unsigned int h = 1080, bool useDepth = true, bool useMs = false, bool useDepthMap2D = false, bool useDepthCube = false);

	~FrameBuffer();

	GLuint getFBO() const { return FBO; }
	GLuint getColor() const { return texColor; }
	GLuint getDepth2D() const { return texDepth2D; }
	GLuint getDepthCube() const { return texDepthCube; }

	std::shared_ptr<Texture> getColorTexture() const { return colorTexture; }
	std::shared_ptr<Texture> getDepthTexture() const { return depthTexture2D; }
	std::shared_ptr<Texture> getDepthCubeTexture() const { return depthCubeTexture; }
};

