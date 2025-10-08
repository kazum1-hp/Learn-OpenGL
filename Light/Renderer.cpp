#include "Renderer.h"

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

Mesh creatLight()
{
    std::vector<float> light = {
        sin((float)glfwGetTime()) * 2.0f, 1.0f, cos((float)glfwGetTime()) * 2.0f
    };

    std::vector<VertexAttribute> attr = {
    {0, 3, GL_FLOAT, GL_FALSE} // position
    };

    return Mesh(light, attr);
}

Mesh creatCube()
{
	std::vector<float> object = {
		// pos(3) + texcoord(2)
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f };

	std::vector<VertexAttribute> attr = {
	{1, 3, GL_FLOAT, GL_FALSE}, // position
	{2, 2, GL_FLOAT, GL_FALSE}  // texcoord
	};
	return Mesh(object, attr);
}

Renderer::Renderer()
	:lightShader("light.vs", "light.fs"),
     objectShader("texture.vs", "texture.fs"),
	 lightMesh(creatLight()),
     objectMesh(creatCube()),
	 texture("ei.png")
{
    lightShader.use();
    objectShader.use();
    objectShader.setUniform("texture1", 0);
}

void Renderer::render()
{
    float t = (float)glfwGetTime();

	/*glm::mat4 model;
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/

    glm::mat4 lightModel;

	glm::mat4 view;
    view = camera.cameraView();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

    lightShader.use();
    lightShader.setUniform("lightModel", lightModel);
    lightShader.setUniform("view", view);
    lightShader.setUniform("projection", projection);
    /*lightShader.setUniform("lightColor", lightColor);*/
    lightMesh.draw();

    objectShader.use();
    //objectShader.setUniform("model", model);
    objectShader.setUniform("view", view);
    objectShader.setUniform("projection", projection);
    

    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * (10 - i);
        model = glm::rotate(model, t * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        objectShader.setUniform("model", model);

        glm::vec3 lightColor = glm::vec3((float)(sin(0.2f * t) * 0.5f + 0.5f),
            (float)(sin(0.7f * t * i) * 0.5f + 0.5f),
            (float)(sin(0.3f * t  * i) * 0.5f + 0.5f));
        objectShader.setUniform("lightColor", lightColor);
        objectMesh.draw();
    }

    //objectMesh.draw();
    
}