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

class Scene;
class ResourceManager;

class Renderer
{
private:
	// --- 外部引用 ---
	Window& window;
	Camera& camera;
	InputManager& input;
	Scene& scene;

	// --- 管线资源 (Renderer 拥有的基础设施) ---
	// 渲染器需要持有它用来完成渲染工作的 Shader，但数据从 ResourceManager 获取
	std::shared_ptr<Shader> modelShader;
	std::shared_ptr<Shader> lightShader;
	std::shared_ptr<Shader> sceneFramebufferShader;
	std::shared_ptr<Shader> skyboxShader;
	std::shared_ptr<Shader> dirShadowShader;
	std::shared_ptr<Shader> pointShadowShader;
	std::shared_ptr<Shader> bloomBlurShader;
	std::shared_ptr<Shader> gBufferShader;
	std::shared_ptr<Shader> lightPassShader;
	std::shared_ptr<Shader> debugShader;
	std::shared_ptr<Shader> gbufferDebugShader;
	std::shared_ptr<Shader> pbrShader;

	// framebuffers
	std::vector<std::unique_ptr<FrameBuffer>> framebuffers;
	std::unique_ptr<FrameBuffer> pingpongFrameBuffer[2];
	std::vector<std::unique_ptr<FrameBuffer>> pointShadowFramebuffers;

	std::shared_ptr<Mesh> screenQuad;
	std::shared_ptr<Mesh> plane;
	std::shared_ptr<Mesh> cube;

	glm::mat4 LightSpaceMatrix;

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
	bool hasARMMap = false;

	float samplerDistance = 1.0f;
	bool useHdr = true;
	bool useBloom = false;
	float exposure = 1.0f;

	bool useDeferred = false;
	bool usePbr = false;

public:
	Renderer(Camera& cam, InputManager& input, Window& win, Scene& scene);
	void init();
	void render(const Scene& scene);
	void renderModel(const Transform& transform, const Model& model, Shader& shader);
	void drawMesh(const Mesh& mesh, Shader& shader) const;
	void drawModel(const Model& model, Shader& shader) const;
	void resizeFrameBuffer();
	void onImGuiRender();

	void forwardPass(const Scene& scene);
	void deferredPass(const Scene& scene);
	void shadowPass(const Scene& scene);
	void geometryPass(const Scene& scene);
	void lightPass(const Scene& scene);
	void postProcessPass(const FrameBuffer& framebuffer);

	void pbrPass(const Scene& scene);
};

