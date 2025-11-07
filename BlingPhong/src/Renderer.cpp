#include "../head/Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

std::vector<float> quadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

std::vector<unsigned int> indices = {
    0, 1, 2, 
    0, 2, 3  
};

std::vector<VertexAttribute> attributes = {
    { 0, 2, GL_FLOAT, GL_FALSE },
    { 1, 2, GL_FLOAT, GL_FALSE }
};

const std::vector<std::string> faces = {
    ("../Assets/skybox2/1.png"),
    ("../Assets/skybox2/3.png"),
    ("../Assets/skybox2/5.png"),
    ("../Assets/skybox2/6.png"),
    ("../Assets/skybox2/2.png"),
    ("../Assets/skybox2/4.png")
};

std::vector<float> planeVertices = {
    // positions            // normals         // texcoords
     10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
    -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
     10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
};

std::vector<unsigned int> p_indices = {
    0, 1, 2,
    0, 2, 3
};

std::vector<VertexAttribute> f_attributes = {
    { 0, 3, GL_FLOAT, GL_FALSE },
    { 1, 3, GL_FLOAT, GL_FALSE },
    { 2, 2, GL_FLOAT, GL_FALSE }
};

Renderer::Renderer(Camera& cam, InputManager& input, Window& win, const std::vector<std::string>& modelPaths)
    :skybox(faces),
     camera(cam),
     input(input),
     window(win)
{
    for (const auto& path : modelPaths) {
        models.push_back(std::make_unique<Model>(path));
    }

    Framebuffers.push_back(std::make_unique<FrameBuffer>(window, true, false));
    Framebuffers.push_back(std::make_unique<FrameBuffer>(window, false, false));
    FrameBuffer& msFrameBuffer = *Framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *Framebuffers[1];

    geometrys.push_back(std::make_unique<Geometry>(quadVertices, indices, attributes));
    geometrys.push_back(std::make_unique<Geometry>(planeVertices, p_indices, f_attributes));
    Geometry& scene = *geometrys[0];
    Geometry& plane = *geometrys[1];

    meshes.push_back(std::make_unique<Mesh>(scene, std::vector<std::shared_ptr<Texture>>{ sceneFrameBuffer.getColorTexture() }));
    std::shared_ptr<Texture> planeTex = std::make_shared <Texture>("../Assets/wood.png");
    planeTex -> bind();
    meshes.push_back(std::make_unique<Mesh>(plane, std::vector<std::shared_ptr<Texture>>{ planeTex }));

    shaders.push_back(std::make_unique<Shader>("shader/light.vs", "shader/light.fs"));    // light shader
    shaders.push_back(std::make_unique<Shader>("shader/framebuffer.vs", "shader/framebuffer.fs"));    // framebuffer shader
    shaders.push_back(std::make_unique<Shader>("shader/skybox.vs", "shader/skybox.fs"));    // skybox shader

    Shader& shader = *shaders[0];
    shader.use();

    materials.push_back(std::make_unique<Material>("material"));
    Material& material = *materials[0];

    shader.setUniform("material.shininess", material.getShininess());

    Shader& skyboxShader = *shaders[2];
    skyboxShader.setUniform("skybox", 0);

    // --- glEnable ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void Renderer::render()
{
    FrameBuffer& msFrameBuffer = *Framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *Framebuffers[1];
    glBindFramebuffer(GL_FRAMEBUFFER, msFrameBuffer.getFBO());
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // skybox
    glDepthFunc(GL_LEQUAL);
    Shader& skyboxShader = *shaders[2];
    skyboxShader.use();

    skyboxShader.setUniform("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
    skyboxShader.setUniform("projection", camera.getProjectionMatrix());
    skyboxShader.setUniform("light", skyboxLight);

    skybox.bind();
    //skybox.draw();
    glDepthFunc(GL_LESS);

    // normal object
    Shader& shader = *shaders[0];
    shader.use();

    // transform matrix
    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("viewPos", camera.getPosition());

    // light 
    shader.setUniform("useBlinnPhong", useBlinnPhong);
    shader.setUniform("plane", 0);

    shader.setUniform("pointLight.enabled", input.pointLightOn);

    //point light
    shader.setUniform("pointLight.ambient", light.getAmbient());
    shader.setUniform("pointLight.diffuse", light.getDiffuse());
    shader.setUniform("pointLight.specular", light.getSpecular());
    shader.setUniform("pointLight.position", light.getLightPos());

    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());
    
    meshes[1]-> draw();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msFrameBuffer.getFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sceneFrameBuffer.getFBO());
    glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(), 
        0, 0, window.getWidth(), window.getHeight(), 
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    // framebuffer
    Shader& framebufferShader = *shaders[1];
    framebufferShader.use();

    framebufferShader.setUniform("effectMode", effectMode);
    framebufferShader.setUniform("offset", offset);
    framebufferShader.setUniform("screenTexture", 0);
    framebufferShader.setUniform("scanPos", scanPos);
    meshes[0] -> draw();

    glEnable(GL_DEPTH_TEST); 
}

void Renderer::onImGuiRender()
{
    ImGui::Begin("Post Processing");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    light.onImGuiRender();
    ImGui::Combo("Effect Mode", &effectMode, "normal\0inversion\0grayscale\0sharpen\0blur\0\0");
    if (effectMode == 3 || effectMode == 4)
    {
        ImGui::SliderFloat("Offset", &offset, 100.0f, 1000.0f);
    }
    ImGui::SliderFloat("Scan Pos", &scanPos, 0.0f, static_cast<float>(window.getWidth()));
    input.onImGuiRender();

    if (ImGui::Checkbox("MSAA", &useMSAA))
    {
        // rebulid FrameBuffer
        Framebuffers[0] = std::make_unique<FrameBuffer>(window, true, useMSAA);
    }
    ImGui::SameLine();
    ImGui::Checkbox("useBinnPhong", &useBlinnPhong);

    ImGui::End();
}