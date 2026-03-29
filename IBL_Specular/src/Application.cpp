#include "../head/Application.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

#include <iostream>

Application::Application(const char* title)
	: camera(),
	  input(camera),
	  window(title, input),
	  renderer(camera, input, window, mainScene),
	  running(true)
{
	if (!window.getWindow())
	{
		running = false;
		return;
	}

	init();
}

void Application::init()
{
	glEnable(GL_DEPTH_TEST);
	// 如果将来有更多初始化（如帧缓冲、后期处理系统），在这里统一注册
	// 例如：
	// renderer.initFrameBuffers();
	// sceneManager.loadDefaultScene();
	// ---------------------------------------------------------
	// 步骤 A: 资源加载 (只做一次)
	// 向 ResourceManager 下单，把模型数据加载到堆内存
	// ---------------------------------------------------------
	auto& res = ResourceManager::GetInstance();

	// 加载 Shader
	res.LoadShader("model", "shader/model.vs", "shader/model.fs");
    res.LoadShader("light", "shader/light.vs", "shader/light.fs");
    res.LoadShader("scene framebuffer", "shader/framebuffer.vs", "shader/framebuffer.fs");
    res.LoadShader("skybox", "shader/skybox.vs", "shader/skybox.fs");
    res.LoadShader("dir shadow", "shader/shadow.vs", "shader/shadow.fs");
    res.LoadShader("point shadow", "shader/pointShadow.vs", "shader/pointShadow.fs", "shader/pointShadow.gs");
    res.LoadShader("bloomBlur", "shader/bloomBlur.vs", "shader/bloomBlur.fs");
    res.LoadShader("gBuffer", "shader/gBuffer.vs", "shader/gBuffer.fs");;
    res.LoadShader("lightPass", "shader/lightPass.vs", "shader/lightPass.fs");
    res.LoadShader("debug", "shader/debug.vs", "shader/debug.fs");
    res.LoadShader("gbuffer debug", "shader/drawDebug.vs", "shader/drawDebug.fs");
	//res.LoadShader("pbr shader", "shader/pbr.vs", "shader/pbr.fs");
	res.LoadShader("background", "shader/background.vs", "shader/background.fs");
	res.LoadShader("irradiance", "shader/irradiance.vs", "shader/irradiance.fs");
	res.LoadShader("prefilter", "shader/prefilter.vs", "shader/prefilter.fs");
	res.LoadShader("brdf", "shader/brdf.vs", "shader/brdf.fs");

	auto model = res.LoadModel("../Assets/blue_metal_plate_4k.gltf/blue_metal_plate_4k.gltf");
	auto material = res.LoadMaterial("material");
	auto environmentMap = res.LoadTexture("../Assets/ferndale_studio_12_4k.hdr", HDR);

	mainScene.AddObject(model, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.3f), material);
	mainScene.AddPointLight(Light(glm::vec3(2.0f, 2.0f, 2.0f), 1.0f, glm::vec3(0.0f, 0.5f, 1.5f), LightType::Point));
	mainScene.AddPointLight(Light(glm::vec3(2.0f, 2.0f, 2.0f), 1.0f, glm::vec3(-4.0f, 0.5f, -3.0f), LightType::Point));
	mainScene.AddPointLight(Light(glm::vec3(2.0f, 2.0f, 2.0f), 1.0f, glm::vec3(3.0f, 0.5f, 1.0f), LightType::Point));
	mainScene.AddPointLight(Light(glm::vec3(2.0f, 2.0f, 2.0f), 1.0f, glm::vec3(-0.8f, 2.4f, -1.0f), LightType::Point));
	mainScene.SetSkybox(environmentMap->getID());
	renderer.init();
}

void Application::initImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::shutdownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::run()
{
	if (!running) return;

	// initial ImGui
	initImGui();

	float lastFrame = 0.0f;
	int drawCall = 1;

	while (!glfwWindowShouldClose(window.getWindow())) {
		
		
		float currentFrame = static_cast<float>(glfwGetTime());
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		update(deltaTime);

		// Clear screen
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //custom color for screen clean
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render(mainScene);
		//std::cout << "DrawCall: " << drawCall << std::endl;
		drawCall++;

		// ImGui new frame 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui content	
		renderer.onImGuiRender();

		// render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers and poll IO events
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
		
		if (input.shouldClose()) {
			glfwSetWindowShouldClose(window.getWindow(), true);
			continue; 
		}
	}
}

void Application::update(float deltaTime)
{
	input.update(window.getWindow(), deltaTime);
	// 如果将来要加入 SceneManager，可以在这里统一 update：
	// sceneManager.update(deltaTime);
}

void Application::shutdown()
{
	glfwTerminate();
}

Application::~Application()
{
	if (running)
	{
		shutdownImGui();
		shutdown();
	}
}