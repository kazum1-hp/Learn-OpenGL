#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

Renderer::Renderer(Camera& cam, InputManager& input, const std::string& modelPath)
    :myModel(modelPath),
     camera(cam),
     input(input),
     shader("light.vs", "light.fs"),
     material()
{
    std::cout << "Creating renderer, GL version: " << glGetString(GL_VERSION) << std::endl;

    shader.use();

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

    //shader.setUniform("spotLight.ambient", light.getAmbient());
    shader.setUniform("spotLight.diffuse", light.getDiffuse());
    shader.setUniform("spotLight.specular", light.getSpecular());

}

void Renderer::render()
{
    std::cout << "Rendering frame..." << std::endl;
    std::cout << "Shader ID: " << shader.ID << std::endl;


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

    if (!myModel.textures.empty())
    {
        myModel.textures[0]->bind(); // diffuse
        if (myModel.textures.size() > 1)
            myModel.textures[1]->bind(); // specular
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    shader.setUniform("model", model);

    std::cout << "Camera pos: " << camera.getPosition().x << ", "
        << camera.getPosition().y << ", "
        << camera.getPosition().z << std::endl;


    glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));

    shader.setUniform("normalMatrix", normalMatrix);

    myModel.draw();
    std::cout << "Draw call executed" << std::endl;

}