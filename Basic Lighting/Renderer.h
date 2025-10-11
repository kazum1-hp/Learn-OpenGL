#pragma once
#include "Window.h"
#include "Shader.h"
#include "Geometry.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"

class Renderer
{
private:
	Geometry geometry;
	Mesh mesh;
	Texture texture;
	Light light;
	Camera& camera;
	Shader shader;
public:
	Renderer(Camera& cam);
	void render();
};

