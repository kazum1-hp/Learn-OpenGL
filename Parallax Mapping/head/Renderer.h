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
#include "Transform.h"
#include "../external/imgui/imgui.h"

#include <vector>
#include <memory>

class Renderer
{
private:
	std::vector<std::unique_ptr<Model>> models;
	std::vector<std::unique_ptr<Geometry>> geometrys;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<FrameBuffer>> framebuffers;
	std::vector<std::unique_ptr<Skybox>> skyboxes;
	Light light;
	Camera& camera;
	InputManager& input;
	std::vector<std::unique_ptr<Shader>> shaders;
	std::vector<std::unique_ptr<Material>> materials;
	Window& window;
	Transform transform;

	int effectMode = 0;
	float skyboxLight = 1.0f;
	float modelLight = 1.0f;
	float offset = 300.0f;

	float scanPos = 0.0f;

	//glm::vec3 normalColor;

	//bool enableInstancing = false;
	//int instance = 1;

	bool useMSAA = true;
	bool useBlinnPhong = true;
	bool useQuadratic = true;
	bool useGamma = true;

	const unsigned int SHADOW_Size = 1024;
	bool parallelShadows = true;
	bool pointShadows = true;

	bool hasNormal = false;
	bool hasHeight = false;
	float height_scale = 0.001f;

	bool useHdr = false;
	float exposure = 1.0f;

public:
	Renderer(Camera& cam, InputManager& input, Window& win, const std::vector<std::string>& modelPaths);
	void render();
	void renderModel(const Transform& transform, const Model& model, Shader& shader);
	void drawMesh(const Mesh& mesh, Shader& shader) const;
	void drawModel(const Model& model, Shader& shader) const;
	void resizeFrameBuffer();
	void onImGuiRender();
};

