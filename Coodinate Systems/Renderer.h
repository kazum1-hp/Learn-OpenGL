#pragma once
#include "shader.h"
#include "Mesh.h"
#include "texture.h"
#include "GLUtils.h"

class Renderer
{
private:
	Shader shader;
	Mesh mesh;
	Texture texture;

public:
	Renderer();
	void render();
};

