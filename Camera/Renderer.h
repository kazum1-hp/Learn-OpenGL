#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "GLUtils.h"
#include "Camera.h"

class Renderer
{
private:
	Shader shader;
	Mesh mesh;
	Texture texture;
	//Camera camera;

public:
	Renderer();
	void render();
};

