#pragma once
#include "Shader.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "InputManager.h"
#include "Window.h"
#include "../external/imgui/imgui.h"

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
	Window& window;

	int effectMode = 0;
	float skyboxLight = 1.0f;
	float modelLight = 1.0f;
	float offset = 300.0f;

	float scanPos = 0.0f;

	glm::vec3 normalColor;

	bool enableInstancing = false;
	int instance = 1;

	bool useMSAA = false;
	bool useBlinnPhong = false;
	bool useQuadratic = false;
	bool useGamma = false;

public:
	Renderer(Camera& cam, InputManager& input, Window& win, const std::vector<std::string>& modelPaths);
	void render();
	void onImGuiRender();
};

