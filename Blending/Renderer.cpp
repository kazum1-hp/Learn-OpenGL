#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

Renderer::Renderer(Camera& cam, InputManager& input, const std::vector<std::string>& modelPaths)
    :camera(cam),
     input(input)
{
    for (const auto& path : modelPaths) {
        models.push_back(std::make_unique<Model>(path));
    }

    shaders.push_back(std::make_unique<Shader>("light.vs", "light.fs"));    // normal shader
    shaders.push_back(std::make_unique<Shader>("mirror.vs", "mirror.fs")); // mirror shader
    shaders.push_back(std::make_unique<Shader>("outline.vs", "outline.fs")); // mirror shader
    Shader& shader = *shaders[0];
    shader.use();

    materials.push_back(std::make_unique<Material>("material"));
    //materials.push_back(std::make_unique<Material>("mirror"));
    Material& material = *materials[0];
    //Material& mirror = *materials[1];

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

    // --- depth test ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::render()
{
    Shader& shader = *shaders[0];
    Shader& mirrorShader = *shaders[1];
    Shader& outlineShader = *shaders[2];

    // normal object
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

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

    if (!model.textures.empty())
    {
        model.textures[0]->bind(); // diffuse
        if (model.textures.size() > 1)
            model.textures[1]->bind(); // specular
    }
    model.draw();

    // mirror
    mirrorShader.use();
    glm::mat4 mirrorModelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 15.0f, 15.0f)); 
    mirrorModelMatrix = glm::rotate(mirrorModelMatrix, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
    mirrorModelMatrix = glm::scale(mirrorModelMatrix, glm::vec3(0.2f));

    mirrorShader.setUniform("model", mirrorModelMatrix);
    mirrorShader.setUniform("view", camera.getViewMatrix());
    mirrorShader.setUniform("projection", camera.getProjectionMatrix());

    mirrorShader.setUniform("mirrorColor", glm::vec3(1.0f));
    
    Model& mirror = *models[1];
    mirror.draw();

    //outline
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);

    outlineShader.use();

    outlineShader.setUniform("model", modelMatrix);
    outlineShader.setUniform("view", camera.getViewMatrix());
    outlineShader.setUniform("projection", camera.getProjectionMatrix());

    outlineShader.setUniform("outlineThickness", 0.15f);
    outlineShader.setUniform("outlineColor", glm::vec3(0.3f, 0.2f, 0.5f));

    model.draw();

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}