#include "../head/Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>


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
     25.0f, -1.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -1.0f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -25.0f, -1.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
     25.0f, -1.0f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
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
    :camera(cam),
     input(input),
     window(win)
{
    for (const auto& path : modelPaths) {
        models.push_back(std::make_unique<Model>(path));
    }

    /*skyboxes.push_back(std::make_unique<Skybox>());
    Skybox& skybox = *skyboxes[0];*/

    framebuffers.push_back(std::make_unique<FrameBuffer>(window, true, true, false, false));
    framebuffers.push_back(std::make_unique<FrameBuffer>(window, false, false, false, false));
    framebuffers.push_back(std::make_unique<FrameBuffer>(SHADOW_Size, SHADOW_Size, false, false, false, true));
    FrameBuffer& msFrameBuffer = *framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *framebuffers[1];
    FrameBuffer& shadowFrameBuffer = *framebuffers[2];

    geometrys.push_back(std::make_unique<Geometry>(quadVertices, indices, attributes));
    geometrys.push_back(std::make_unique<Geometry>(planeVertices, p_indices, f_attributes));
    geometrys.push_back(std::make_unique<Geometry>(cube_vertices, cube_indices, cube_attributes));
    Geometry& scene = *geometrys[0];
    Geometry& plane = *geometrys[1];
    Geometry& cube  = *geometrys[2];

    meshes.push_back(std::make_unique<Mesh>(scene, std::vector<std::shared_ptr<Texture>>{ sceneFrameBuffer.getColorTexture() }));
    std::shared_ptr<Texture> planeTex = std::make_shared <Texture>("../Assets/wood.png");
    planeTex -> bind();
    meshes.push_back(std::make_unique<Mesh>(plane, std::vector<std::shared_ptr<Texture>>{ planeTex }));
    meshes.push_back(std::make_unique<Mesh>(cube, std::vector<std::shared_ptr<Texture>>{ planeTex }));

    shaders.push_back(std::make_unique<Shader>("shader/light.vs", "shader/light.fs"));    // light shader
    shaders.push_back(std::make_unique<Shader>("shader/framebuffer.vs", "shader/framebuffer.fs"));    // framebuffer shader
    shaders.push_back(std::make_unique<Shader>("shader/skybox.vs", "shader/skybox.fs"));    // skybox shader
    shaders.push_back(std::make_unique<Shader>("shader/shadow.vs", "shader/shadow.fs"));    // shadow shader
    shaders.push_back(std::make_unique<Shader>("shader/pointShadow.vs", "shader/pointShadow.fs", "shader/pointShadow.gs"));    // shadow shader

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

    // --- glEnable ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void Renderer::render()
{
    glm::mat4 model = glm::mat4(1.0f);
    
    light.Update();

    FrameBuffer& msFrameBuffer = *framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *framebuffers[1];
    FrameBuffer& shadowFrameBuffer = *framebuffers[2];
    
    glViewport(0, 0, SHADOW_Size, SHADOW_Size);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer.getFBO());
    glClear(GL_DEPTH_BUFFER_BIT);

    Shader& shadowShader = *shaders[4];
    shadowShader.use();

    for (GLuint i = 0; i < 6; ++i)
    {
        shadowShader.setUniform("shadowMatrices[" + std::to_string(i) + "]", light.getPerspTransMatrix(i));
    }

    shadowShader.setUniform("far_plane", light.getFar());
    shadowShader.setUniform("lightPos", light.getLightPos());
    
    glm::mat4 planeModel = model;
    shadowShader.setUniform("model", planeModel);
    meshes[1]->draw();

    glm::mat4 cube1Model = glm::scale(model, glm::vec3(10.0f));
    shadowShader.setUniform("model", cube1Model);
    //meshes[2]->draw();

    glm::mat4 cube2Model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    cube2Model = glm::scale(cube2Model, glm::vec3(0.5f));
    shadowShader.setUniform("model", cube2Model);
    //meshes[2]->draw();

    glm::mat4 cube3Model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    cube3Model = glm::scale(cube3Model, glm::vec3(0.75f));
    shadowShader.setUniform("model", cube3Model);
    //meshes[2]->draw();

    glm::mat4 cube4Model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    cube4Model = glm::scale(cube4Model, glm::vec3(0.5f));
    shadowShader.setUniform("model", cube4Model);
    //meshes[2]->draw();

    glm::mat4 cube5Model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    cube5Model = glm::scale(cube5Model, glm::vec3(0.5f));
    shadowShader.setUniform("model", cube5Model);
    //meshes[2]->draw();

    glm::mat4 cube6Model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    cube6Model = glm::rotate(cube6Model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    cube6Model = glm::scale(cube6Model, glm::vec3(0.75f));
    shadowShader.setUniform("model", cube6Model);
    //meshes[2]->draw();

    glm::mat4 raidenModel = glm::translate(model, glm::vec3(-1.5f, -1.0f, 1.0f));
    raidenModel = glm::scale(raidenModel, glm::vec3(0.1));
    shadowShader.setUniform("model", raidenModel);
    Model& raiden = *models[0];
    raiden.draw();

    glm::mat4 catModel = glm::translate(raidenModel, glm::vec3(-20.0f, 0.0f, 0.0f));
    shadowShader.setUniform("model", catModel);
    Model& cat = *models[1];
    cat.draw();

    glm::mat4 hanabiModel = glm::translate(raidenModel, glm::vec3(20.0f, 0.0f, 0.0f));
    shadowShader.setUniform("model", hanabiModel);
    Model& hanabi = *models[2];
    hanabi.draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, msFrameBuffer.getFBO());
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // object
    Shader& shader = *shaders[0];
    shader.use();

    shader.setUniform("shadows", shadows);
    shader.setUniform("far_plane", light.getFar());
    // transform matrix
    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("viewPos", camera.getPosition());

    // light 
    shader.setUniform("useBlinnPhong", useBlinnPhong);
    shader.setUniform("useQuadratic", useQuadratic);
    shader.setUniform("modelLight", modelLight);
    shader.setUniform("pointLight.enabled", input.isPointLightOn());

    //point light
    shader.setUniform("pointLight.ambient", light.getAmbient() * light.getColor());
    shader.setUniform("pointLight.diffuse", light.getDiffuse() * light.getColor());
    shader.setUniform("pointLight.specular", light.getSpecular() * light.getColor());
    shader.setUniform("pointLight.position", light.getLightPos());

    // plane
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowFrameBuffer.getDepthCube());

    shader.setUniform("textures", 0);
    shader.setUniform("shadowMap", 1);

    shader.setUniform("model", planeModel);

    meshes[1]->draw();

    // cubes

    shader.setUniform("model", cube1Model);
    //glDisable(GL_CULL_FACE);
    //shader.setUniform("reverse_normals", 1);
    //meshes[2]->draw();
    //shader.setUniform("reverse_normals", 0);
    //glEnable(GL_CULL_FACE);

    shader.setUniform("model", cube2Model);
    //meshes[2]->draw();

    shader.setUniform("model", cube3Model);
   //meshes[2]->draw();

    shader.setUniform("model", cube4Model);
    //meshes[2]->draw();

    shader.setUniform("model", cube5Model);
    //meshes[2]->draw();

    shader.setUniform("model", cube6Model);
    //meshes[2]->draw();

    shader.setUniform("model", raidenModel);
    
    raiden.draw();

    shader.setUniform("model", catModel);

    cat.draw();

    shader.setUniform("model", hanabiModel);

    hanabi.draw();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msFrameBuffer.getFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sceneFrameBuffer.getFBO());
    glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(), 
        0, 0, window.getWidth(), window.getHeight(), 
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
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
        framebuffers[0] = std::make_unique<FrameBuffer>(window, true, useMSAA, false);
    }
    ImGui::SameLine();
    ImGui::Checkbox("useBinnPhong", &useBlinnPhong);
    ImGui::SameLine();
    ImGui::Checkbox("useQuadratic", &useQuadratic);
    ImGui::SameLine();
    ImGui::Checkbox("useGamma", &useGamma);

    ImGui::End();
}