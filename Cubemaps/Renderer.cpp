#include "Renderer.h"
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
    ("../Assets/skybox/right.jpg"),
    ("../Assets/skybox/left.jpg"),
    ("../Assets/skybox/top.jpg"),
    ("../Assets/skybox/bottom.jpg"),
    ("../Assets/skybox/front.jpg"),
    ("../Assets/skybox/back.jpg")
};

Renderer::Renderer(Camera& cam, InputManager& input, const std::vector<std::string>& modelPaths)
    :skybox(faces),
     camera(cam),
     input(input)
{
    for (const auto& path : modelPaths) {
        models.push_back(std::make_unique<Model>(path));
    }

    Framebuffers.push_back(std::make_unique<FrameBuffer>());
    FrameBuffer& sceneFrameBuffer = *Framebuffers[0];

    geometrys.push_back(std::make_unique<Geometry>(quadVertices, indices, attributes));
    Geometry& scene = *geometrys[0];

    meshes.push_back(std::make_unique<Mesh>(scene, std::vector<std::shared_ptr<Texture>>{ sceneFrameBuffer.getColorTexture() }));

    shaders.push_back(std::make_unique<Shader>("model.vs", "model.fs"));    // normal shader
    shaders.push_back(std::make_unique<Shader>("framebuffer.vs", "framebuffer.fs"));    // framebuffer shader
    shaders.push_back(std::make_unique<Shader>("skybox.vs", "skybox.fs"));    // skybox shader

    Shader& shader = *shaders[0];
    shader.use();

    materials.push_back(std::make_unique<Material>("material"));
    Material& material = *materials[0];

    shader.setUniform("material.diffuse", 0);
    shader.setUniform("material.specular", 1);
    shader.setUniform("material.shininess", material.getShininess());

    shader.setUniform("parallelLight.ambient", light.getAmbient());
    shader.setUniform("parallelLight.diffuse", light.getDiffuse());
    shader.setUniform("parallelLight.specular", light.getSpecular());

    shader.setUniform("pointLight.ambient", light.getAmbient());
    shader.setUniform("pointLight.diffuse", light.getDiffuse());
    shader.setUniform("pointLight.specular", light.getSpecular());
    shader.setUniform("pointLight.constant", 1.0f);
    shader.setUniform("pointLight.linear", 0.09f);
    shader.setUniform("pointLight.quadratic", 0.032f);

    shader.setUniform("spotLight.ambient", light.getAmbient());
    shader.setUniform("spotLight.diffuse", light.getDiffuse());
    shader.setUniform("spotLight.specular", light.getSpecular());

    Shader& skyboxShader = *shaders[2];
    skyboxShader.setUniform("skybox", 0);

    // --- depth test ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::render()
{
    FrameBuffer& sceneFrameBuffer = *Framebuffers[0];
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer.getFBO());
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // normal object
    Shader& shader = *shaders[0];
    shader.use();

    shader.setUniform("parallelLight.enabled", input.parallelLightOn);
    shader.setUniform("pointLight.enabled", input.pointLightOn);
    shader.setUniform("spotLight.enabled", input.spotLightOn);

    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("viewPos", camera.getPosition());

    light.update();

    shader.setUniform("pointLight.position", light.getLightPos());

    shader.setUniform("parallelLight.direction", light.getLightDir());

    shader.setUniform("spotLight.position", camera.getPosition());
    shader.setUniform("spotLight.direction", camera.getFront());
    shader.setUniform("spotLight.cutOff", glm::cos(glm::radians(5.5f)));
    shader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(7.5f)));

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -20.0f));
    shader.setUniform("model", modelMatrix);

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

    shader.setUniform("normalMatrix", normalMatrix);

    Model& model = *models[0];

    model.draw();

    // skybox
    glDepthFunc(GL_LEQUAL);
    Shader& skyboxShader = *shaders[2];
    skyboxShader.use();

    skyboxShader.setUniform("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
    skyboxShader.setUniform("projection", camera.getProjectionMatrix());

    skybox.bind();
    skybox.draw();
    glDepthFunc(GL_LESS);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    // framebuffer
    Shader& framebufferShader = *shaders[1];
    framebufferShader.use();

    framebufferShader.setUniform("screenTexture", 0);
    Mesh& sceneMesh = *meshes[0];

    sceneMesh.draw();

    glEnable(GL_DEPTH_TEST);
    
}