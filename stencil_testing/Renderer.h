#pragma once
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "InputManager.h"
#include <vector>
#include <memory>

class Renderer
{
private:
	std::vector<std::unique_ptr<Model>> models;
	Light light;
	Camera& camera;
	InputManager& input;
	std::vector<std::unique_ptr<Shader>> shaders;
	Material material;

public:
	Renderer(Camera& cam, InputManager& input, const std::vector<std::string>& modelPaths);
	void render();
};

