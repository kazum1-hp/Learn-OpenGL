#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "GLUtils.h"
#include "Camera.h"
#include "Light.h"

class Renderer
{
private:
	Shader shader;
	Mesh mesh;
	Texture texture;
	Light light;

public:
	Renderer();
	void render();
};

