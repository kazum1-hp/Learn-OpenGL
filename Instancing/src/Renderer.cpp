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
    Geometry& scene = *geometrys[0];

    meshes.push_back(std::make_unique<Mesh>(scene, std::vector<std::shared_ptr<Texture>>{ sceneFrameBuffer.getColorTexture() }));

    shaders.push_back(std::make_unique<Shader>("shader/model.vs", "shader/model.fs"));    // normal shader
    shaders.push_back(std::make_unique<Shader>("shader/framebuffer.vs", "shader/framebuffer.fs"));    // framebuffer shader
    shaders.push_back(std::make_unique<Shader>("shader/skybox.vs", "shader/skybox.fs"));    // skybox shader
    shaders.push_back(std::make_unique<Shader>("shader/normal.vs", "shader/normal.fs", "shader/normal.gs"));    // geometry shader
    shaders.push_back(std::make_unique<Shader>("shader/instance.vs", "shader/instance.fs"));    // instance shader

    Shader& shader = *shaders[0];
    shader.use();

    materials.push_back(std::make_unique<Material>("material"));
    Material& material = *materials[0];

    shader.setUniform("material.diffuse", 0);
    shader.setUniform("material.specular", 1);
    shader.setUniform("material.shininess", material.getShininess());

    shader.setUniform("pointLight.constant", 1.0f);
    shader.setUniform("pointLight.linear", 0.09f);
    shader.setUniform("pointLight.quadratic", 0.032f);

    Shader& skyboxShader = *shaders[2];
    skyboxShader.setUniform("skybox", 0);

    Shader& geoShader = *shaders[3];
    geoShader.use();

    geoShader.setUniform("material.diffuse", 0);
    geoShader.setUniform("material.specular", 1);
    geoShader.setUniform("material.shininess", material.getShininess());

    Shader& instanceShader = *shaders[4];
    instanceShader.use();

    instanceShader.setUniform("material.diffuse", 0);
    instanceShader.setUniform("material.specular", 1);
    instanceShader.setUniform("material.shininess", material.getShininess());

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
    skybox.draw();
    glDepthFunc(GL_LESS);

    Model& rock = *models[0];

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -20.0f));

    if (!enableInstancing)
    {
        // normal object
        Shader& shader = *shaders[0];
        shader.use();

        // transform matrix
        shader.setUniform("view", camera.getViewMatrix());
        shader.setUniform("projection", camera.getProjectionMatrix());
        shader.setUniform("viewPos", camera.getPosition());

        // light 
        shader.setUniform("parallelLight.enabled", input.parallelLightOn);
        shader.setUniform("pointLight.enabled", input.pointLightOn);
        shader.setUniform("spotLight.enabled", input.spotLightOn);

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

        // spot light
        //shader.setUniform("spotLight.ambient", light.getAmbient() * light.getColor());
        shader.setUniform("spotLight.diffuse", light.getDiffuse() * light.getColor());
        shader.setUniform("spotLight.specular", light.getSpecular() * light.getColor());
        shader.setUniform("spotLight.position", camera.getPosition());
        shader.setUniform("spotLight.direction", camera.getFront());
        shader.setUniform("spotLight.cutOff", glm::cos(glm::radians(5.5f)));
        shader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(8.5f)));

        shader.setUniform("model", modelMatrix);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

        shader.setUniform("normalMatrix", normalMatrix);
        shader.setUniform("modelLight", modelLight);

        rock.draw();
    }
    else 
    {
        // instance shader
        Shader& instanceShander = *shaders[4];
        instanceShander.use();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        std::vector<glm::mat4> transforms;
        //srand(glfwGetTime());
        float radius = 50.0;
        float offset = 2.5f;
        for (int i = 0; i < instance; i++)
        {
            float angle = (float)i / (float)instance * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            glm::mat4 model = glm::translate(modelMatrix, glm::vec3(x, y, z));

            // 2. 缩放：在 0.05 和 0.25f 之间缩放
            float scale = (rand() % 20) / 100.0f + 0.05f;
            model = glm::scale(model, glm::vec3(scale));

            // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
            float rotAngle = static_cast<float>(rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            //glm::mat4 model = glm::translate(modelMatrix, glm::vec3(x, y, 0.0f));
            glm::mat4 MVP = projection * view * model;
            transforms.push_back(MVP);
        }

        rock.enableInstancing(transforms);
        rock.draw();
    }

    Model& planet = *models[1];
    Shader& shader = *shaders[0];
    shader.use();
    glm::mat4 model = glm::translate(modelMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    shader.setUniform("model", model);
    shader.setUniform("view", camera.getViewMatrix());
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("modelLight", modelLight);

    planet.draw();

    // geometry shader
    Shader& geoShader = *shaders[3];
    geoShader.use();

    geoShader.setUniform("view", camera.getViewMatrix());
    geoShader.setUniform("projection", camera.getProjectionMatrix());
    geoShader.setUniform("model", modelMatrix);
    geoShader.setUniform("modelLight", modelLight);
    geoShader.setUniform("time", static_cast<float>(glfwGetTime()));
    geoShader.setUniform("normalColor", normalColor);
    //model.draw();

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

    //light.onImGuiRender();
    //ImGui::ColorEdit3("Normal Color", glm::value_ptr(normalColor));
    ImGui::Combo("Effect Mode", &effectMode, "normal\0inversion\0grayscale\0sharpen\0blur\0\0");
    if (effectMode == 3 || effectMode == 4)
    {
        ImGui::SliderFloat("Offset", &offset, 100.0f, 1000.0f);
    }
    ImGui::SliderFloat("Skybox Light", &skyboxLight, 0.1f, 1.0f);
    ImGui::SliderFloat("Model Light", &modelLight, 0.1f, 1.0f);
    ImGui::SliderFloat("Scan Pos", &scanPos, 0.0f, static_cast<float>(window.getWidth()));
    input.onImGuiRender();

    ImGui::SeparatorText("Instance Rendering");
    ImGui::Checkbox("Enable Instancing", &enableInstancing);
    if (enableInstancing)
    {
        const int minInstance = 1, maxInstance = 10000;
        static int step = 1;  
        // Step input
        ImGui::Text("Step:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##Step", &step, 0, 0);
        if (step < 1) step = 1;
        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));

        ImGui::Text("Instance Count:");
        ImGui::SameLine();

        if (ImGui::Button("-##Decrease"))
        {
            instance -= step;
            if (instance < minInstance) instance = minInstance;
        }
        ImGui::SameLine();

        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##InstanceInput", &instance, 0, 0);
        if (instance < minInstance) instance = minInstance;
        if (instance > maxInstance) instance = maxInstance;

        ImGui::SameLine();
        if (ImGui::Button("+##Increase"))
        {
            instance += step;
            if (instance > maxInstance) instance = maxInstance;
        }
        ImGui::PopStyleVar();
    }

    if (ImGui::Checkbox("MSAA", &useMSAA))
    {
        // rebulid FrameBuffer
        Framebuffers[0] = std::make_unique<FrameBuffer>(window, true, useMSAA);
    }

    ImGui::End();
}