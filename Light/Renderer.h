#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "GLUtils.h"
#include "Camera.h"

class Renderer
{
private:
	Shader lightShader;
	Shader objectShader;
	Mesh lightMesh;
	Mesh objectMesh;
	Texture texture;

public:
	Renderer();
	void render();
};

