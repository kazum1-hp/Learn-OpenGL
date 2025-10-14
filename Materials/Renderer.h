#pragma once
#include "Shader.h"
#include "Geometry.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

class Renderer
{
private:
	Geometry geometry;
	Mesh mesh;
	Texture texture;
	Light light;
	Camera& camera;
	Shader shader;
	Material material;
public:
	Renderer(Camera& cam);
	void render();
};

