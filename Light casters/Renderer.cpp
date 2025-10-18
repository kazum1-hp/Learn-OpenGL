#include "Renderer.h"
#include <GLFW/glfw3.h>

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};


std::vector<float> object = {
    // pos(3) + texcoord(2) + normal(3)
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

std::vector<VertexAttribute> attr = {
    {0, 3, GL_FLOAT, GL_FALSE}, // position
    {1, 2, GL_FLOAT, GL_FALSE}, // texcoord
    {2, 3, GL_FLOAT, GL_FALSE}  // normal
};


Renderer::Renderer(Camera& cam, InputManager& input)
    :geometry(object, attr),
     mesh(geometry),
    /* textureVec{Texture("Assets/ei.png", 0), Texture("Assets/container2_specular.png", 1) },*/
     light(),
     camera(cam),
     input(input),
     shader("light.vs", "light.fs"),
     material()
{
    textureVec.emplace_back("Assets/ei.png", 0);
    textureVec.emplace_back("Assets/container2_specular.png", 1);

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

    /*textureVec.push_back(std::make_shared<Texture>("Assets/ei.png", 0));
    textureVec.push_back(std::make_shared<Texture>("Assets/container2_specular.png", 1));*/
}

void Renderer::render()
{
    shader.use();

    shader.setUniform("parallelLight.enabled", input.parallelLightOn);
    shader.setUniform("pointLight.enabled", input.pointLightOn);
    shader.setUniform("spotLight.enabled", input.spotLightOn);

    for (auto& tex : textureVec) {
        tex.bind();
    }

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

    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setUniform("model", model);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));
        shader.setUniform("normalMatrix", normalMatrix);

        mesh.draw();
    }
}