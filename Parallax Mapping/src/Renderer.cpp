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
    
    transform.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    transform.setScale(glm::vec3(10.0f));
    /*skyboxes.push_back(std::make_unique<Skybox>());
    Skybox& skybox = *skyboxes[0];*/

    framebuffers.push_back(std::make_unique<FrameBuffer>(window, true, true, false, false));
    framebuffers.push_back(std::make_unique<FrameBuffer>(window, false, false, false, false));
    framebuffers.push_back(std::make_unique<FrameBuffer>(SHADOW_Size, SHADOW_Size, false, false, true, false));
    framebuffers.push_back(std::make_unique<FrameBuffer>(SHADOW_Size, SHADOW_Size, false, false, false, true));
    FrameBuffer& msFrameBuffer = *framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *framebuffers[1];
    FrameBuffer& parallelShadowFrameBuffer = *framebuffers[2];
    FrameBuffer& pointShadowFrameBuffer = *framebuffers[3];

    geometrys.push_back(std::make_unique<Geometry>(quadVertices, indices, attributes));
    geometrys.push_back(std::make_unique<Geometry>(planeVertices, p_indices, f_attributes));
    geometrys.push_back(std::make_unique<Geometry>(cube_vertices, cube_indices, cube_attributes));
    Geometry& scene = *geometrys[0];
    Geometry& plane = *geometrys[1];
    Geometry& cube  = *geometrys[2];

    std::shared_ptr<Texture> colortexture = std::make_shared <Texture>(sceneFrameBuffer.getColor());
    meshes.push_back(std::make_unique<Mesh>(scene, std::vector<std::shared_ptr<Texture>>{ colortexture }));
    std::shared_ptr<Texture> planeTex = std::make_shared <Texture>("../Assets/wood.png");
    meshes.push_back(std::make_unique<Mesh>(plane, std::vector<std::shared_ptr<Texture>>{ planeTex }));

    shaders.push_back(std::make_unique<Shader>("shader/light.vs", "shader/light.fs"));    // light shader
    shaders.push_back(std::make_unique<Shader>("shader/framebuffer.vs", "shader/framebuffer.fs"));    // framebuffer shader
    shaders.push_back(std::make_unique<Shader>("shader/skybox.vs", "shader/skybox.fs"));    // skybox shader
    shaders.push_back(std::make_unique<Shader>("shader/shadow.vs", "shader/shadow.fs"));    // shadow shader
    shaders.push_back(std::make_unique<Shader>("shader/pointShadow.vs", "shader/pointShadow.fs", "shader/pointShadow.gs"));    // shadow shader

    Shader& shader = *shaders[0];
    shader.use();

    materials.push_back(std::make_unique<Material>("material"));
    Material& material = *materials[0];

    //shader.setUniform("textures", 0);
    //shader.setUniform("depthMap", 1);
    //shader.setUniform("shadowMap", 2);

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
    glm::mat4 LightSpaceMatrix = light.getOrthoMatrix() * light.getOrthoViewMatrix();
    glm::mat4 model(1.0f);
    Model& baseball = *models[0];
    Mesh& plane = *meshes[1];
    //light.Update();

    FrameBuffer& msFrameBuffer = *framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *framebuffers[1];
    FrameBuffer& parallelShadowFrameBuffer = *framebuffers[2];
    FrameBuffer& pointShadowFrameBuffer = *framebuffers[3];
    
    glViewport(0, 0, SHADOW_Size, SHADOW_Size);

    if (input.isParallelLightOn())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, parallelShadowFrameBuffer.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);

        Shader& shadowShader = *shaders[3];
        shadowShader.use();

        shadowShader.setUniform("lightSpaceMatrix", LightSpaceMatrix);
        shadowShader.setUniform("model", model);
        plane.draw();
        renderModel(transform, baseball, shadowShader);
    }

    if (input.isPointLightOn())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pointShadowFrameBuffer.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);

        Shader& shadowShader = *shaders[4];
        shadowShader.use();

        for (GLuint i = 0; i < 6; ++i)
        {
            shadowShader.setUniform("shadowMatrices[" + std::to_string(i) + "]", light.getPerspTransMatrix(i));
        }

        shadowShader.setUniform("far_plane", light.getFar());
        shadowShader.setUniform("lightPos", light.getLightPos());
        shadowShader.setUniform("model", model);
        plane.draw();
        renderModel(transform, baseball, shadowShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, msFrameBuffer.getFBO());
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // object
    Shader& shader = *shaders[0];
    shader.use();

    // transform matrix
    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("viewPos", camera.getPosition());

    // light 
    shader.setUniform("useBlinnPhong", useBlinnPhong);
    shader.setUniform("useQuadratic", useQuadratic);
    shader.setUniform("modelLight", modelLight);

    // paralleLight
    shader.setUniform("parallelLight.ambient", light.getAmbient() * light.getColor());
    shader.setUniform("parallelLight.diffuse", light.getDiffuse() * light.getColor());
    shader.setUniform("parallelLight.specular", light.getSpecular() * light.getColor());
    shader.setUniform("parallelLight.direction", light.getLightDir());
    shader.setUniform("parallelLight.enabled", input.isParallelLightOn());
    shader.setUniform("lightSpaceMatrix", LightSpaceMatrix);
    shader.setUniform("parallelShadows", parallelShadows);

    //point light
    shader.setUniform("pointLight.ambient", light.getAmbient() * light.getColor());
    shader.setUniform("pointLight.diffuse", light.getDiffuse() * light.getColor());
    shader.setUniform("pointLight.specular", light.getSpecular() * light.getColor());
    shader.setUniform("pointLight.position", light.getLightPos());
    shader.setUniform("pointLight.enabled", input.isPointLightOn());
    shader.setUniform("far_plane", light.getFar());
    shader.setUniform("pointShadows", pointShadows);

    // plane
    //shader.setUniform("textures", 0);

    if (input.isParallelLightOn())
    {
        pointShadows = false;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, parallelShadowFrameBuffer.getDepth2D());

        shader.setUniform("depthMap", 0);
        //std::cerr << "Bind Texture slot: 0" << " ID: " << parallelShadowFrameBuffer.getDepth2D() << std::endl;
    }
    else pointShadows = true;

    if (input.isPointLightOn())
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowFrameBuffer.getDepthCube());

        shader.setUniform("shadowMap", 1);
        //std::cerr << "Bind Texture slot: 1" << " ID: " << pointShadowFrameBuffer.getDepthCube() << std::endl;
    }
    drawMesh(plane, shader);
    shader.setUniform("model", model);
    shader.setUniform("hasNormalMap", false);
    plane.draw();
    drawModel(baseball, shader);
    shader.setUniform("hasNormalMap", hasNormal);
    renderModel(transform, baseball, shader);

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneFrameBuffer.getColor());
    meshes[0]->draw();

    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawMesh(const Mesh& mesh, Shader& shader) const
{

    GLuint slot = 2; // 0 = depthMap, 1 = shadowMap

    for (const auto& tex : mesh.getTexture())
    {
        tex->bind(slot);

        std::string uniformName;

        switch (tex->getType())
        {
            case TextureType::Diffuse:
                uniformName = "diffuse";
                break;
            case TextureType::Specular:
                uniformName = "specular";
                break;
            case TextureType::Normal:
                uniformName = "normal";
                //hasNormal = true;
                break;
            case TextureType::Height:
                uniformName = "height";
                break;
        }

        shader.setUniform(uniformName, static_cast<int>(slot));
        //std::cerr << "Bind Texture slot: " << slot << " ID: " << tex->getID() << " Path: " << tex->getPath() << " uniform: " << uniformName << std::endl;
        slot++;
    }
    
    //shader.setUniform("hasNormalMap", hasNormal);
    //mesh.draw();
}

void Renderer::drawModel(const Model& model, Shader& shader) const
{
    for (auto& mesh : model.meshes)
    {
        drawMesh(*mesh, shader);
    }
}

void Renderer::renderModel(const Transform& transform, const Model& model, Shader& shader)
{
    glm::mat4 modelMatrix = transform.getModelMatrix();

    //Model& treasure = *models[0];
    shader.setUniform("model", modelMatrix);
    //drawModel(treasure, shader);
    model.draw();
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
    ImGui::SameLine();
    ImGui::Checkbox("useNormal", &hasNormal);

    transform.onImGuiRender();

    ImGui::End();
}