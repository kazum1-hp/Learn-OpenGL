#pragma once
#include "Shader.h"
#include "Geometry.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "InputManager.h"
#include <vector>
#include <memory>

class Renderer
{
private:
	Geometry geometry;
	Mesh mesh;
	/*Texture texture;
	Texture texture2;*/
	std::vector<std::shared_ptr<Texture>> textureVec;
	Light light;
	Camera& camera;
	InputManager& input;
	Shader shader;
	Material material;
public:
	Renderer(Camera& cam, InputManager& input);
	void render();
};

