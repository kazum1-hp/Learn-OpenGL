#include "../head/Application.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

Application::Application(const char* title)
	: camera(),
	  window(title, camera, input),  
	  input(camera),
	  renderer(camera, input, window, { "../Assets/raiden/raiden.pmx"}),
	  running(true)
{
	if (!window.getWindow())
	{
		running = false;
		return;
	}
}

void Application::run()
{
	if (!running) return;

	// initial ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark(); 

	// initial platform/render bind
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window.getWindow())) {
		// ImGui new frame 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		float currentFrame = static_cast<float>(glfwGetTime());
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		update(deltaTime);

		// Clear screen
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //custom color for screen clean

		renderer.render();

		// ImGui content	
		renderer.onImGuiRender();

		// render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers and poll IO events
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();

		input.closeWindow(window.getWindow());
	}
}

void Application::update(float deltaTime)
{
	input.cursorState(window.getWindow());
	input.moveControl(window.getWindow(), camera, deltaTime);
	input.lightControl(window.getWindow());
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate(); //release resources
}