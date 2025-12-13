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
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left   3
    // front face
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left    4
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right   5
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right  6
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left   7
    // left face
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right  8
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left   9
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left    10
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right   11
    // right face
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left   12
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right   13
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right       14  
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     15
     // bottom face
     -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right 16
      1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left  17
      1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left   18
     -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right  19
     // top face
     -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left  20
      1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right  21
      1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right         22
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
     window(win),
     dirLight(glm::vec3(1.0f), glm::vec3(-2.2f, -2.0f, -2.3f), LightType::Directional)
{
    for (const auto& path : modelPaths) {
        models.push_back(std::make_unique<Model>(path));
    }
    
    pointLights.push_back(Light(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.5f, 1.5f), LightType::Point));
    pointLights.push_back(Light(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(-4.0f, 0.5f, -3.0f), LightType::Point));
    pointLights.push_back(Light(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(3.0f, 0.5f, 1.0f), LightType::Point));
    pointLights.push_back(Light(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(-0.8f, 2.4f, -1.0f), LightType::Point));

    transform.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    transform.setScale(glm::vec3(1.0f));
    /*skyboxes.push_back(std::make_unique<Skybox>());
    Skybox& skybox = *skyboxes[0];*/

    framebuffers.push_back(std::make_unique<FrameBuffer>(window, /*useDepth*/true, /*useMs*/false, /*useDepthMap2D*/false, /*useDepthCube*/false, /*useHdr*/useHdr, 2));
    framebuffers.push_back(std::make_unique<FrameBuffer>(SHADOW_Size, SHADOW_Size, false, false, true, false, false));

    pingpongFrameBuffer[0] = std::make_unique<FrameBuffer>(window, false, false, false, false, true);
    pingpongFrameBuffer[1] = std::make_unique<FrameBuffer>(window, false, false, false, false, true);

    for (int i = 0; i < pointLights.size(); i++)
    {
        pointShadowFramebuffers.push_back(
            std::make_unique<FrameBuffer>(SHADOW_Size, SHADOW_Size,
                false, false,
                false,  // useDepthMap2D
                true,   // useDepthCube
                false)  // useHdr
        );
    }

    window.onFramebufferResize = [this]() {
        this->resizeFrameBuffer();
        };

    geometrys.push_back(std::make_unique<Geometry>(quadVertices, indices, attributes));
    geometrys.push_back(std::make_unique<Geometry>(planeVertices, p_indices, f_attributes));
    geometrys.push_back(std::make_unique<Geometry>(cube_vertices, cube_indices, cube_attributes));
    Geometry& scene = *geometrys[0];
    Geometry& plane = *geometrys[1];
    Geometry& cube  = *geometrys[2];

    meshes.push_back(std::make_unique<Mesh>(scene));
    std::shared_ptr<Texture> planeTex = std::make_shared <Texture>("../Assets/wood.png");
    meshes.push_back(std::make_unique<Mesh>(plane, std::vector<std::shared_ptr<Texture>>{ planeTex }));
    meshes.push_back(std::make_unique<Mesh>(cube));

    shaders.push_back(std::make_unique<Shader>("shader/model.vs", "shader/model.fs"));    // model shader
    shaders.push_back(std::make_unique<Shader>("shader/light.vs", "shader/light.fs"));    // light shader
    shaders.push_back(std::make_unique<Shader>("shader/framebuffer.vs", "shader/framebuffer.fs"));    // framebuffer shader
    shaders.push_back(std::make_unique<Shader>("shader/skybox.vs", "shader/skybox.fs"));    // skybox shader
    shaders.push_back(std::make_unique<Shader>("shader/shadow.vs", "shader/shadow.fs"));    // shadow shader
    shaders.push_back(std::make_unique<Shader>("shader/pointShadow.vs", "shader/pointShadow.fs", "shader/pointShadow.gs"));    // shadow shader
    shaders.push_back(std::make_unique<Shader>("shader/bloomBlur.vs", "shader/bloomBlur.fs"));    // bloomBlur shader

    Shader& shader = *shaders[0];
    shader.use();
    
    materials.push_back(std::make_unique<Material>("material"));
    Material& material = *materials[0];

    shader.setUniform("material.shininess", material.getShininess());
    for (int i = 0; i < pointLights.size(); i++)
    {
        auto& light = pointLights[i];
        std::string base = "pointLight[" + std::to_string(i) + "]";

        shader.setUniform(base + ".constant", 1.0f);
        shader.setUniform(base + ".linear", 0.09f);
        shader.setUniform(base + ".quadratic", 0.032f);
    }

    Shader& blurShader = *shaders[6];
    blurShader.use();
    blurShader.setUniform("image", 0);

    // --- glEnable ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void Renderer::render()
{
    glm::mat4 LightSpaceMatrix = dirLight.getOrthoMatrix() * dirLight.getOrthoViewMatrix();
    glm::mat4 model(1.0f);
    Model& baseball = *models[0];
    Mesh& plane = *meshes[1];
    Mesh& cube = *meshes[2];

    FrameBuffer& hdrFrameBuffer = *framebuffers[0];
    FrameBuffer& parallelShadowFrameBuffer = *framebuffers[1];
    
    glViewport(0, 0, SHADOW_Size, SHADOW_Size);

    if (input.isParallelLightOn())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, parallelShadowFrameBuffer.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);

        Shader& shadowShader = *shaders[4];
        shadowShader.use();

        shadowShader.setUniform("lightSpaceMatrix", LightSpaceMatrix);
        shadowShader.setUniform("model", model);
        plane.draw();
        renderModel(transform, baseball, shadowShader);
    }

    if (input.isPointLightOn())
    {
        Shader& shadowShader = *shaders[5];
        shadowShader.use();

        for (int i = 0; i < pointLights.size(); i++)
        {
            FrameBuffer& fbo = *pointShadowFramebuffers[i];

            glBindFramebuffer(GL_FRAMEBUFFER, fbo.getFBO());
            glClear(GL_DEPTH_BUFFER_BIT);

            for (GLuint j = 0; j < 6; ++j)
            {
                shadowShader.setUniform("shadowMatrices[" + std::to_string(j) + "]", pointLights[i].getPerspTransMatrix(j));
            }

            shadowShader.setUniform("far_plane", pointLights[i].getFar());
            shadowShader.setUniform("lightPos", pointLights[i].getLightPos());

            shadowShader.setUniform("model", model);
            plane.draw();

            renderModel(transform, baseball, shadowShader);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer.getFBO());
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
    shader.setUniform("parallelLight.ambient", dirLight.getAmbient() * dirLight.getColor());
    shader.setUniform("parallelLight.diffuse", dirLight.getDiffuse() * dirLight.getColor());
    shader.setUniform("parallelLight.specular", dirLight.getSpecular() * dirLight.getColor());
    shader.setUniform("parallelLight.direction", dirLight.getLightDir());
    shader.setUniform("parallelLight.enabled", input.isParallelLightOn());
    shader.setUniform("lightSpaceMatrix", LightSpaceMatrix);
    shader.setUniform("parallelShadows", parallelShadows);

    if (input.isParallelLightOn())
    {
        pointShadows = false;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, parallelShadowFrameBuffer.getDepth2D());

        shader.setUniform("depthMap", 0);
    }
    else pointShadows = true;

    //point light
    shader.setUniform("pointShadows", pointShadows);

    for (int i = 0; i < pointLights.size(); i++)
    {
        std::string base = "pointLight[" + std::to_string(i) + "]";

        shader.setUniform(base + ".ambient", pointLights[i].getAmbient() * pointLights[i].getColor());
        shader.setUniform(base + ".diffuse", pointLights[i].getDiffuse() * pointLights[i].getColor());
        shader.setUniform(base + ".specular", pointLights[i].getSpecular() * pointLights[i].getColor());
        shader.setUniform(base + ".position", pointLights[i].getLightPos());
        shader.setUniform(base + ".enabled", pointLights[i].lightOn() && input.isPointLightOn());
        shader.setUniform("far_plane", pointLights[i].getFar());

        if (input.isPointLightOn())
        {
            glActiveTexture(GL_TEXTURE1 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowFramebuffers[i]->getDepthCube());

            shader.setUniform("shadowMap[" + std::to_string(i) + "]", 1 + i);
        }
    }

    drawMesh(plane, shader);
    shader.setUniform("model", model);
    shader.setUniform("hasNormalMap", false);
    shader.setUniform("hasHeightMap", false);
    plane.draw();

    drawModel(baseball, shader);
    shader.setUniform("hasNormalMap", hasNormal);
    shader.setUniform("hasHeightMap", hasHeight);
    shader.setUniform("height_scale", height_scale);
    renderModel(transform, baseball, shader);

    Shader& LightShader = *shaders[1];
    LightShader.use();

    // transform matrix
    LightShader.setUniform("view", camera.getViewMatrix());
    LightShader.setUniform("projection", camera.getProjectionMatrix());

    for (int i = 0; i < pointLights.size(); i++)
    {
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(model, pointLights[i].getLightPos());
        lightModel = glm::scale(lightModel, glm::vec3(0.25f));
        LightShader.setUniform("model", lightModel);
        LightShader.setUniform("lightColor", pointLights[i].getColor());
        LightShader.setUniform("enabled", pointLights[i].lightOn() && input.isPointLightOn());
        cube.draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;
    Shader& blurShader = *shaders[6];
    blurShader.use();
    blurShader.setUniform("samplerDistance", samplerDistance);

    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[horizontal]->getFBO());
        blurShader.setUniform("horizontal", horizontal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? hdrFrameBuffer.getColor(1) : pingpongFrameBuffer[!horizontal]->getColor());  // bind texture of other framebuffer (or scene if first iteration)
        meshes[0]->draw();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // framebuffer
    Shader& framebufferShader = *shaders[2];
    framebufferShader.use();

    framebufferShader.setUniform("effectMode", effectMode);
    framebufferShader.setUniform("offset", offset);
    framebufferShader.setUniform("screenTexture", 0);
    framebufferShader.setUniform("blur", 1);
    framebufferShader.setUniform("scanPos", scanPos);
    framebufferShader.setUniform("useGamma", useGamma);
    framebufferShader.setUniform("useHdr", useHdr);
    framebufferShader.setUniform("useBloom", useBloom);
    framebufferShader.setUniform("exposure", exposure);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrFrameBuffer.getColor());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongFrameBuffer[!horizontal]->getColor());
    meshes[0]->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawMesh(const Mesh& mesh, Shader& shader) const
{

    GLuint slot = 6; // 0 = depthMap, 1-5 = shadowMap

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
        slot++;
    }
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

    shader.setUniform("model", modelMatrix);

    model.draw();
}

void Renderer::resizeFrameBuffer()
{
    FrameBuffer& msFrameBuffer = *framebuffers[0];
    FrameBuffer& sceneFrameBuffer = *framebuffers[1];
    msFrameBuffer.resize(window.getWidth(), window.getHeight());
    sceneFrameBuffer.resize(window.getWidth(), window.getHeight());

    camera.aspect = (float)window.getWidth() / (float)window.getHeight();
}

void Renderer::onImGuiRender()
{
    ImGui::Begin("Post Processing");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    //dirLight.dirOnImGuiRender();
    for (int i = 0; i < pointLights.size(); ++i)
    {
        pointLights[i].pointOnImGuiRender(i);
    }


    //ImGui::Combo("Effect Mode", &effectMode, "normal\0inversion\0grayscale\0sharpen\0blur\0\0");
    //if (effectMode == 3 || effectMode == 4)
    //{
    //    ImGui::SliderFloat("Offset", &offset, 100.0f, 1000.0f);
    //}
    ImGui::SliderFloat("Model Light", &modelLight, 0.1f, 1.0f);
    //ImGui::SliderFloat("Scan Pos", &scanPos, 0.0f, static_cast<float>(window.getWidth()));
    input.onImGuiRender();

    //ImGui::SameLine();
    ImGui::Checkbox("useBinnPhong", &useBlinnPhong);
    ImGui::SameLine();
    ImGui::Checkbox("useQuadratic", &useQuadratic);
    ImGui::SameLine();
    ImGui::Checkbox("useGamma", &useGamma);
    ImGui::SameLine();
    ImGui::Checkbox("useNormal", &hasNormal);
    
    ImGui::SliderFloat("height_scale", &height_scale, 0.0005f, 0.01f);
    ImGui::SameLine();
    ImGui::Checkbox("useHeight", &hasHeight);
    
    ImGui::SliderFloat("Exposure", &exposure, 0.01f, 10.0f);
    ImGui::SameLine();
    ImGui::Checkbox("useHdr", &useHdr);

    ImGui::SliderFloat("samplerDistance", &samplerDistance, 0.01f, 10.0f);
    ImGui::SameLine();
    ImGui::Checkbox("useBloom", &useBloom);

    transform.onImGuiRender();

    ImGui::End();
}