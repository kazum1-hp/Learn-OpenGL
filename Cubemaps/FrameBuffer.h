#pragma once

#include "Window.h"
#include "Texture.h"
#include "Mesh.h"

class FrameBuffer
{
private:
	int width, height;
	GLuint FBO, RBO;
	GLuint texColor;
	std::shared_ptr<Texture> colorTexture;

	void init(int w, int h, bool useDepth);

public:
	FrameBuffer(Window& window, bool useDepth = true);

	FrameBuffer(int w = 1920, int h = 1080, bool useDepth = true);

	~FrameBuffer();

	GLuint getFBO() const { return FBO; }

	std::shared_ptr<Texture> getColorTexture() const { return colorTexture; }
};

