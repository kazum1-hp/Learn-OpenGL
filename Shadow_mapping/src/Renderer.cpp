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
    ("../Assets/genshin1/px.png"),
    ("../Assets/genshin1/nx.png"),
    ("../Assets/genshin1/py.png"),
    ("../Assets/genshin1/ny.png"),
    ("../Assets/genshin1/pz.png"),
    ("../Assets/genshin1/nz.png")
};

std::vector<float> planeVertices = {
    // positions            // normals         // texcoords
     25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
     25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
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

std::vector<float> cube_vertices {
    // back face
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left    0
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right  1
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right  2       
    // 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right  1
    //-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left    0
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left   3
    // front face
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left    4
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right   5
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right  6
   //  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right  6
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left   7
   // -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left    4
    // left face
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right  8
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left   9
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left    10
   // -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left    10
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right   11
   // -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right  8
    // right face
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left   12
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right   13
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right       14  
    // 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right      13
    // 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left   12
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     15
     // bottom face
     -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right 16
      1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left  17
      1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left   18
    //  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left   18
     -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right  19
    // -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right 16
     // top face
     -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left  20
      1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right  21
      1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right         22
     // 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right  21
    // -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left  20
     -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left     23      
};

std::vector<unsigned int> cube_indices = {
    0, 1, 2,
    1, 0, 3,
    4, 5, 6,
    6, 7, 4,
    8, 9, 10,
    10, 11, 8,
    12, 13, 14,
    13, 12, 15,
    16, 17, 18,
    18, 19, 16,
    20, 21, 22,
    21, 20, 23
};

std::vector<VertexAttribute> cube_attributes = {
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
    Model& raiden = *models[0];
    Model& cat = *models[1];

    Framebuffers.push_back(std::make_unique<FrameBuffer>(window, true, false, false));
    Framebuffers.push_back(std::make_unique<FrameBuffer>(window, false, false, false));
    Framebuffers.push_back(std::make_unique<FrameBuffer>(window, false, false, true));
    FrameBuffer& msFrameBuffer = *Framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *Framebuffers[1];
    FrameBuffer& shadowFrameBuffer = *Framebuffers[2];

    geometrys.push_back(std::make_unique<Geometry>(quadVertices, indices, attributes));
    geometrys.push_back(std::make_unique<Geometry>(planeVertices, p_indices, f_attributes));
    geometrys.push_back(std::make_unique<Geometry>(cube_vertices, cube_indices, cube_attributes));
    Geometry& scene = *geometrys[0];
    Geometry& plane = *geometrys[1];
    Geometry& cube  = *geometrys[2];

    meshes.push_back(std::make_unique<Mesh>(scene, std::vector<std::shared_ptr<Texture>>{ sceneFrameBuffer.getColorTexture() }));
    std::shared_ptr<Texture> planeTex = std::make_shared <Texture>("../Assets/wood.png");
    planeTex -> bind();
    meshes.push_back(std::make_unique<Mesh>(plane, std::vector<std::shared_ptr<Texture>>{ planeTex, shadowFrameBuffer.getDepthTexture() }));
    meshes.push_back(std::make_unique<Mesh>(cube, std::vector<std::shared_ptr<Texture>>{ planeTex, shadowFrameBuffer.getDepthTexture() }));

    for (auto& mesh : raiden.meshes)
    {
        mesh->getTexture().push_back(shadowFrameBuffer.getDepthTexture());
    }
    for (auto& mesh : cat.meshes)
    {
        mesh->getTexture().push_back(shadowFrameBuffer.getDepthTexture());
    }

    shaders.push_back(std::make_unique<Shader>("shader/light.vs", "shader/light.fs"));    // light shader
    shaders.push_back(std::make_unique<Shader>("shader/framebuffer.vs", "shader/framebuffer.fs"));    // framebuffer shader
    shaders.push_back(std::make_unique<Shader>("shader/skybox.vs", "shader/skybox.fs"));    // skybox shader
    shaders.push_back(std::make_unique<Shader>("shader/shadow.vs", "shader/shadow.fs"));    // shadow shader

    Shader& shader = *shaders[0];
    shader.use();

    materials.push_back(std::make_unique<Material>("material"));
    Material& material = *materials[0];

    shader.setUniform("textures", 0);
    shader.setUniform("shadowMap", 1);
    shader.setUniform("material.shininess", material.getShininess());
    shader.setUniform("pointLight.constant", 1.0f);
    shader.setUniform("pointLight.linear", 0.09f);
    shader.setUniform("pointLight.quadratic", 0.032f);

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
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 LightSpaceMatrix = light.getProjectionMatrix() * light.getViewMatrix();

    light.Update();

    FrameBuffer& msFrameBuffer = *Framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *Framebuffers[1];
    FrameBuffer& shadowFrameBuffer = *Framebuffers[2];
    
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer.getFBO());
    glClear(GL_DEPTH_BUFFER_BIT);

    Shader& shadowShader = *shaders[3];
    shadowShader.use();

    glm::mat4 planeModel = model;
    shadowShader.setUniform("lightSpaceMatrix", LightSpaceMatrix);
    shadowShader.setUniform("model", planeModel);
    meshes[1]->draw();

    glm::mat4 cube1Model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    shadowShader.setUniform("model", cube1Model);
    meshes[2]->draw();

    glm::mat4 cube2Model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    shadowShader.setUniform("model", cube2Model);
    meshes[2]->draw();

    glm::mat4 cube3Model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    cube3Model = glm::rotate(cube3Model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    cube3Model = glm::scale(cube3Model, glm::vec3(0.25));
    shadowShader.setUniform("model", cube3Model);
    meshes[2]->draw();
    
   /* glm::mat4 raidenModel = glm::translate(model, glm::vec3(-1.5f, -0.5f, 1.0f));
    raidenModel = glm::scale(raidenModel, glm::vec3(0.1));
    shadowShader.setUniform("model", raidenModel);*/

    /*Model& raiden = *models[0];
    for (auto& mesh : raiden.meshes)
    {
        mesh->draw();
    }*/

    /*glm::mat4 catModel = glm::translate(raidenModel, glm::vec3(-10.0f, 0.0f, 10.0f));

    Model& cat = *models[1];
    for (auto& mesh : cat.meshes)
    {
        mesh->draw();
    }*/

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, window.getWidth(), window.getHeight());
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

    // object
    Shader& shader = *shaders[0];
    shader.use();

    // transform matrix
    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("viewPos", camera.getPosition());

    shader.setUniform("lightSpaceMatrix", LightSpaceMatrix);

    // light 
    shader.setUniform("useBlinnPhong", useBlinnPhong);
    shader.setUniform("useQuadratic", useQuadratic);
    shader.setUniform("modelLight", modelLight);
    shader.setUniform("parallelLight.enabled", input.parallelLightOn);
    shader.setUniform("pointLight.enabled", input.pointLightOn);

    // paralleLight
    shader.setUniform("parallelLight.ambient", light.getAmbient() * light.getColor());
    shader.setUniform("parallelLight.diffuse", light.getDiffuse() * light.getColor());
    shader.setUniform("parallelLight.specular", light.getSpecular() * light.getColor());
    shader.setUniform("parallelLight.direction", light.getLightDir());

    //point light
    shader.setUniform("pointLight.ambient", light.getAmbient() * light.getColor());
    shader.setUniform("pointLight.diffuse", light.getDiffuse() * light.getColor());
    shader.setUniform("pointLight.specular", light.getSpecular() * light.getColor());
    shader.setUniform("pointLight.position", light.getLightPos());

    // plane
    shader.setUniform("model", planeModel);
    meshes[1]->draw();

    // cubes
    shader.setUniform("model", cube1Model);
    meshes[2]->draw();

    shader.setUniform("model", cube2Model);
    meshes[2]->draw();

    shader.setUniform("model", cube3Model);
    meshes[2]->draw();

    //shader.setUniform("model", raidenModel);
    
    //raiden.draw();

    //shader.setUniform("model", catModel);

    //cat.draw();

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
    framebufferShader.setUniform("useGamma", useGamma);
    meshes[0]->draw();

    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
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
    ImGui::SliderFloat("Model Light", &modelLight, 0.1f, 1.0f);
    ImGui::SliderFloat("Scan Pos", &scanPos, 0.0f, static_cast<float>(window.getWidth()));
    input.onImGuiRender();

    if (ImGui::Checkbox("MSAA", &useMSAA))
    {
        // rebulid FrameBuffer
        Framebuffers[0] = std::make_unique<FrameBuffer>(window, true, useMSAA);
    }
    ImGui::SameLine();
    ImGui::Checkbox("useBinnPhong", &useBlinnPhong);
    ImGui::SameLine();
    ImGui::Checkbox("useQuadratic", &useQuadratic);
    ImGui::SameLine();
    ImGui::Checkbox("useGamma", &useGamma);

    ImGui::End();
}