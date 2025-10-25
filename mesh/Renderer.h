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
	//std::vector<std::shared_ptr<Texture>> textureVec;
	Light light;
	Camera& camera;
	InputManager& input;
	Shader shader;
	Material material;
public:
	Renderer(Camera& cam, InputManager& input, const std::vector<std::string>& modelPaths);
	void render();
};

