#pragma once
#include "Shader.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "Skybox.h"
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
	std::vector<std::unique_ptr<Geometry>> geometrys;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<FrameBuffer>> Framebuffers;
	Skybox skybox;
	Light light;
	Camera& camera;
	InputManager& input;
	std::vector<std::unique_ptr<Shader>> shaders;
	std::vector<std::unique_ptr<Material>> materials;

public:
	Renderer(Camera& cam, InputManager& input, const std::vector<std::string>& modelPaths);
	void render();
};

