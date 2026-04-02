#include "../head/Renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "../head/ResourceManager.h"
#include "../head/Scene.h"

Renderer::Renderer(Camera& cam, InputManager& input, Window& win, Scene& scene)
    :camera(cam),
     input(input),
     window(win),
     scene(scene)
{
    framebuffers.push_back(std::make_unique<FrameBuffer>(window, /*useDepth*/true, /*useMs*/false, /*useDepthMap2D*/false, /*useDepthCube*/false, /*useHdr*/useHdr, 2));
    framebuffers.push_back(std::make_unique<FrameBuffer>(SHADOW_Size, SHADOW_Size, false, false, true, false, false));
    framebuffers.push_back(std::make_unique<FrameBuffer>(window, /*useDepth*/false, /*useMs*/false, /*useDepthMap2D*/false, /*useDepthCube*/false, /*useHdr*/true, 4, /*useGbuffer*/true));
    framebuffers.push_back(std::make_unique<FrameBuffer>(window, /*useDepth*/true, /*useMs*/false, /*useDepthMap2D*/false, /*useDepthCube*/false, /*useHdr*/useHdr, 2));
    //framebuffers.push_back(std::make_unique<FrameBuffer>(resolution, resolution, /*useDepth*/true, /*useMs*/false, /*useDepthMap2D*/false, /*useDepthCube*/false, /*useHdr*/false, 0));

    pingpongFrameBuffer[0] = std::make_unique<FrameBuffer>(window, false, false, false, false, true);
    pingpongFrameBuffer[1] = std::make_unique<FrameBuffer>(window, false, false, false, false, true);

    window.onFramebufferResize = [this]() {
        this->resizeFrameBuffer();
        };

    // --- glEnable ---
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::init()
{
    // 1. Get ResourceManager instance
    auto& res = ResourceManager::GetInstance();

    // 2. Load pipeline core Shader
    // Assuming these shaders have already been preloaded into the manager by external or GameInit, or are being loaded for the first time here.
    modelShader = res.GetShader("model");
    lightShader = res.GetShader("light");
    sceneFramebufferShader = res.GetShader("scene framebuffer");
    skyboxShader = res.GetShader("skybox");
    dirShadowShader = res.GetShader("dir shadow");
    pointShadowShader = res.GetShader("point shadow");
    bloomBlurShader = res.GetShader("bloomBlur");
    gBufferShader = res.GetShader("gBuffer");;
    lightPassShader = res.GetShader("lightPass");
    debugShader = res.GetShader("debug");
    gbufferDebugShader = res.GetShader("gbuffer debug");
    backgroundShader = res.GetShader("background");
    irradianceShader = res.GetShader("irradiance");
    prefilterShader = res.GetShader("prefilter");
    brdfShader = res.GetShader("brdf");

    // Get fullscreen Quad 
    screenQuad = res.GetScreenQuad();
    plane = res.GetPlane();
    cube = res.GetCube();

    LightSpaceMatrix = scene.GetDirLight().getOrthoMatrix() * scene.GetDirLight().getOrthoViewMatrix();

    for (int i = 0; i < scene.GetPointLights().size(); i++)
    {
        pointShadowFramebuffers.push_back(
            std::make_unique<FrameBuffer>(SHADOW_Size, SHADOW_Size,
                false, false,
                false,  // useDepthMap2D
                true,   // useDepthCube
                false)  // useHdr
        );
    }

    modelShader->use();
    modelShader->setUniform("irradianceMap", 9);
    modelShader->setUniform("prefilterMap", 10);
    modelShader->setUniform("brdfLUT", 11);

    for (int i = 0; i < scene.GetPointLights().size(); i++)
    {
        std::string base = "pointLight[" + std::to_string(i) + "]";

        modelShader->setUniform(base + ".constant", 1.0f);
        modelShader->setUniform(base + ".linear", 0.09f);
        modelShader->setUniform(base + ".quadratic", 0.032f);
    }

    bloomBlurShader->use();
    bloomBlurShader->setUniform("image", 0);

    backgroundShader->use();
    backgroundShader->setUniform("environmentMap", 0);  

    // defered rendering lightPass
    lightPassShader->use();
    lightPassShader->setUniform("gPosition", 0);
    lightPassShader->setUniform("gNormal", 1);
    lightPassShader->setUniform("gAlbedoSpec", 2);
    lightPassShader->setUniform("gGeoNormal", 3);
    lightPassShader->setUniform("gDepth", 4);

    lightPassShader->setUniform("material.shininess", res.GetMaterial("material")->getShininess());
    for (int i = 0; i < scene.GetPointLights().size(); i++)
    {
        std::string base = "pointLight[" + std::to_string(i) + "]";

        lightPassShader->setUniform(base + ".constant", 1.0f);
        lightPassShader->setUniform(base + ".linear", 0.09f);
        lightPassShader->setUniform(base + ".quadratic", 0.032f);
    }
}

void Renderer::prepareEnvironment(Environment& env)
{
    if (!env.asset) return;

    if (env.maps.isGenerated &&
        env.maps.lastHDR == env.asset->hdrTexture)
        return;

    generateIBLMaps(env);

    env.maps.lastHDR = env.asset->hdrTexture;
    env.maps.isGenerated = true;
}

void Renderer::generateIBLMaps(Environment& env)
{
    auto& res = ResourceManager::GetInstance();

    GLuint envMap = env.asset->hdrTexture;

    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    glGenTextures(1, &env.maps.envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::array<glm::mat4, 6> projViewMatrix = res.calculateCubeMatrices(glm::vec3(0.0f));
    skyboxShader->use();
    skyboxShader->setUniform("equirectangularMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, envMap);

    glViewport(0, 0, resolution, resolution);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glDisable(GL_CULL_FACE);
    for (unsigned int i = 0; i < 6; ++i)
    {
        skyboxShader->setUniform("projView", projViewMatrix[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env.maps.envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    glGenTextures(1, &env.maps.irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irradianceShader->use();
    irradianceShader->setUniform("environmentMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.envCubemap);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader->setUniform("projView", projViewMatrix[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env.maps.irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    // --------------------------------------------------------------------------------
    glGenTextures(1, &env.maps.prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    // ----------------------------------------------------------------------------------------------------
    prefilterShader->use();
    prefilterShader->setUniform("environmentMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader->setUniform("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader->setUniform("projView", projViewMatrix[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env.maps.prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cube->draw();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // pbr: generate a 2D LUT from the BRDF equations used.
    // ----------------------------------------------------
    glGenTextures(1, &env.maps.brdfLUT);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, env.maps.brdfLUT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, env.maps.brdfLUT, 0);

    glViewport(0, 0, 512, 512);
    brdfShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    screenQuad->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, window.getWidth(), window.getHeight());
}

void Renderer::render(Scene& scene)
{
    prepareEnvironment(scene.GetEnvironment());

    if (useShadows)
        shadowPass(scene);
    else
    {
        parallelShadows = false;
        pointShadows = false;
    }

    FrameBuffer& hdrFrameBuffer = *framebuffers[0];
    FrameBuffer& lightPassFrameBuffer = *framebuffers[3];

    if (!useDeferred)
    {        
        if (usePostProcess)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, hdrFrameBuffer.getFBO());
            forwardPass(scene);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            postProcessPass(hdrFrameBuffer);
        }

        else
            forwardPass(scene);
    }
    else
    {
        deferredPass(scene);
        postProcessPass(lightPassFrameBuffer);
    }
}

void Renderer::shadowPass(Scene& scene)
{
    //glm::mat4 LightSpaceMatrix = dirLight.getOrthoMatrix() * dirLight.getOrthoViewMatrix();
    glm::mat4 model(1.0f);

    FrameBuffer& parallelShadowFrameBuffer = *framebuffers[1];

    glViewport(0, 0, SHADOW_Size, SHADOW_Size);

    if (input.isParallelLightOn())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, parallelShadowFrameBuffer.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);

        dirShadowShader->use();

        dirShadowShader->setUniform("lightSpaceMatrix", LightSpaceMatrix);
        dirShadowShader->setUniform("model", model);
        glDisable(GL_CULL_FACE);
        plane->draw();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        for (const auto& obj : scene.GetObjects()) {
            renderModel(obj.transform, *obj.model, *dirShadowShader);
        }
        glCullFace(GL_BACK);
    }

    if (input.isPointLightOn())
    {
        pointShadowShader->use();

        for (int i = 0; i < scene.GetPointLights().size(); i++)
        {
            FrameBuffer& fbo = *pointShadowFramebuffers[i];

            glBindFramebuffer(GL_FRAMEBUFFER, fbo.getFBO());
            glClear(GL_DEPTH_BUFFER_BIT);

            for (GLuint j = 0; j < 6; ++j)
            {
                pointShadowShader->setUniform("shadowMatrices[" + std::to_string(j) + "]", scene.GetPointLights()[i].getPerspTransMatrix(j));
            }

            pointShadowShader->setUniform("far_plane", scene.GetPointLights()[i].getFar());
            pointShadowShader->setUniform("lightPos", scene.GetPointLights()[i].getLightPos());

            pointShadowShader->setUniform("model", model);
            glDisable(GL_CULL_FACE);
            plane->draw();
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            for (const auto& obj : scene.GetObjects()) {
                renderModel(obj.transform, *obj.model, *pointShadowShader);
            }
            glCullFace(GL_BACK);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, window.getWidth(), window.getHeight());
}

void Renderer::forwardPass(Scene& scene)
{
    glm::mat4 model(1.0f);

    FrameBuffer& parallelShadowFrameBuffer = *framebuffers[1];

    auto& env = scene.GetEnvironment();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // object
    modelShader->use();

    modelShader->setUniform("usePost", usePostProcess);

    modelShader->setUniform("aoBias", aoBias);
    modelShader->setUniform("roughnessBias", roughnessBias);
    modelShader->setUniform("metallicBias", metallicBias);

    // transform matrix
    modelShader->setUniform("view", camera.getViewMatrix());
    modelShader->setUniform("projection", camera.getProjectionMatrix());
    modelShader->setUniform("viewPos", camera.getPosition());

    // light 
    modelShader->setUniform("useBlinnPhong", useBlinnPhong);
    modelShader->setUniform("useQuadratic", useQuadratic);
    modelShader->setUniform("modelLight", modelLight);

    // paralleLight
    modelShader->setUniform("parallelLight.color", scene.GetDirLight().getColor());
    modelShader->setUniform("parallelLight.direction", scene.GetDirLight().getLightDir());
    modelShader->setUniform("parallelLight.intensity", scene.GetDirLight().getIntensity());
    modelShader->setUniform("parallelLight.enabled", input.isParallelLightOn());
    modelShader->setUniform("lightSpaceMatrix", LightSpaceMatrix);
    modelShader->setUniform("parallelShadows", parallelShadows);
    modelShader->setUniform("pointShadows", pointShadows);

    if (useShadows)
    {
        if (input.isParallelLightOn())
        {
            pointShadows = false;
            parallelShadows = true;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, parallelShadowFrameBuffer.getDepth2D());

            modelShader->setUniform("depthMap", 0);
        }
        else pointShadows = true;
    }

    //point light

    for (int i = 0; i < scene.GetPointLights().size(); i++)
    {
        std::string base = "pointLight[" + std::to_string(i) + "]";

        modelShader->setUniform(base + ".color", scene.GetPointLights()[i].getColor());
        modelShader->setUniform(base + ".position", scene.GetPointLights()[i].getLightPos());
        modelShader->setUniform(base + ".intensity", scene.GetPointLights()[i].getIntensity());
        modelShader->setUniform(base + ".enabled", scene.GetPointLights()[i].lightOn() && input.isPointLightOn());
        modelShader->setUniform("far_plane", scene.GetPointLights()[i].getFar());

        if (input.isPointLightOn() && useShadows)
        {
            glActiveTexture(GL_TEXTURE1 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowFramebuffers[i]->getDepthCube());

            modelShader->setUniform("shadowMap[" + std::to_string(i) + "]", 1 + i);
        }
    }

    if (drawPlane)
    {
        drawMesh(*plane, *modelShader);
        modelShader->setUniform("model", model);
        modelShader->setUniform("hasNormalMap", false);
        modelShader->setUniform("hasHeightMap", false);
        modelShader->setUniform("hasARMMap", false);
        glDisable(GL_CULL_FACE);
        plane->draw();
        glEnable(GL_CULL_FACE);
    }

    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.irradianceMap);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.prefilterMap);
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, env.maps.brdfLUT);

    for (const auto& obj : scene.GetObjects()) {
        drawModel(*obj.model, *modelShader);
        modelShader->setUniform("hasNormalMap", hasNormal);
        modelShader->setUniform("hasHeightMap", hasHeight);
        modelShader->setUniform("height_scale", height_scale);
        modelShader->setUniform("hasARMMap", true);
        renderModel(obj.transform, *obj.model, *modelShader);
    }

    if (drawLights)
    {
        // draw cube lights
        lightShader->use();

        ////// transform matrix
        lightShader->setUniform("view", camera.getViewMatrix());
        lightShader->setUniform("projection", camera.getProjectionMatrix());

        for (int i = 0; i < scene.GetPointLights().size(); i++)
        {
            glm::mat4 lightModel(1.0f);
            lightModel = glm::translate(model, scene.GetPointLights()[i].getLightPos());
            lightModel = glm::scale(lightModel, glm::vec3(0.25f));
            lightShader->setUniform("model", lightModel);
            lightShader->setUniform("lightColor", scene.GetPointLights()[i].getColor());
            lightShader->setUniform("enabled", scene.GetPointLights()[i].lightOn() && input.isPointLightOn());
            if (scene.GetPointLights()[i].lightOn() && input.isPointLightOn())
            {
                cube->draw();
            }
        }
    }

    glDisable(GL_CULL_FACE); // or glCullFace(GL_FRONT)

    backgroundShader->use();
    backgroundShader->setUniform("view", camera.getViewMatrix());
    backgroundShader->setUniform("projection", camera.getProjectionMatrix());
    backgroundShader->setUniform("usePost", usePostProcess);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.envCubemap);
    cube->draw();

    glEnable(GL_CULL_FACE);
}

void Renderer::postProcessPass(const FrameBuffer& framebuffer)
{
    // bloom
    //FrameBuffer& hdrFrameBuffer = *framebuffers[0];

    glDisable(GL_DEPTH_TEST);

    bool horizontal = true, first_iteration = true;
    unsigned int amount = 5;

    if (useBloom)
    {   
        bloomBlurShader->use();
        bloomBlurShader->setUniform("samplerDistance", samplerDistance);

        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[horizontal]->getFBO());
            bloomBlurShader->setUniform("horizontal", horizontal);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? framebuffer.getColor(1) : pingpongFrameBuffer[!horizontal]->getColor());  // bind texture of other framebuffer (or scene if first iteration)
            screenQuad->draw();
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    sceneFramebufferShader->use();

    sceneFramebufferShader->setUniform("effectMode", effectMode);
    sceneFramebufferShader->setUniform("offset", offset);
    sceneFramebufferShader->setUniform("screenTexture", 0);
    sceneFramebufferShader->setUniform("blur", 1);
    sceneFramebufferShader->setUniform("scanPos", scanPos);
    sceneFramebufferShader->setUniform("useGamma", useGamma);
    sceneFramebufferShader->setUniform("useHdr", useHdr);
    sceneFramebufferShader->setUniform("useBloom", useBloom);
    sceneFramebufferShader->setUniform("exposure", exposure);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer.getColor(0));

    if (useBloom)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongFrameBuffer[!horizontal]->getColor());
    }

    screenQuad->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::deferredPass(Scene& scene)
{
    geometryPass(scene);
    lightPass(scene);
}

void Renderer::geometryPass(Scene& scene)
{
    glm::mat4 model(1.0f);

    FrameBuffer& gFrameBuffer = *framebuffers[2];
    GLuint gBuffer = gFrameBuffer.getFBO();

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gBufferShader->use();
    // uniform setting
    gBufferShader->setUniform("view", camera.getViewMatrix());
    gBufferShader->setUniform("projection", camera.getProjectionMatrix());
    gBufferShader->setUniform("viewPos", camera.getPosition());

    /*drawMesh(*plane, *gBufferShader);
    gBufferShader->setUniform("model", model);
    gBufferShader->setUniform("hasNormalMap", false);
    gBufferShader->setUniform("hasHeightMap", false);
    glDisable(GL_CULL_FACE);
    plane->draw();
    glEnable(GL_CULL_FACE);*/

    for (const auto& obj : scene.GetObjects()) {
        drawModel(*obj.model, *gBufferShader);
        gBufferShader->setUniform("hasNormalMap", hasNormal);
        gBufferShader->setUniform("hasHeightMap", hasHeight);
        gBufferShader->setUniform("height_scale", height_scale);
        renderModel(obj.transform, *obj.model, *gBufferShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::lightPass(Scene& scene)
{
    glm::mat4 model(1.0f);

    FrameBuffer& parallelShadowFrameBuffer = *framebuffers[1];
    FrameBuffer& gFrameBuffer = *framebuffers[2];
    FrameBuffer& lightPassFrameBuffer = *framebuffers[3];

    GLuint gBuffer = gFrameBuffer.getFBO();
    GLuint gPosition = gFrameBuffer.getColor(0);
    GLuint gNormal = gFrameBuffer.getColor(1);
    GLuint gAlbedoSpec = gFrameBuffer.getColor(2);
    GLuint gGeoNormal = gFrameBuffer.getColor(3);
    GLuint gDepth = gFrameBuffer.getGDepth();

    glBindFramebuffer(GL_FRAMEBUFFER, lightPassFrameBuffer.getFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // lightPass.use()
    //Shader& lightPassShader = *shaders[8];
    lightPassShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gGeoNormal);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, gDepth);

    // uniform settings
    lightPassShader->setUniform("viewPos", camera.getPosition());
    // light 
    lightPassShader->setUniform("useBlinnPhong", useBlinnPhong);
    lightPassShader->setUniform("useQuadratic", useQuadratic);
    lightPassShader->setUniform("modelLight", modelLight);

    // paralleLight
    lightPassShader->setUniform("parallelLight.ambient", scene.GetDirLight().getAmbient() * scene.GetDirLight().getColor());
    lightPassShader->setUniform("parallelLight.diffuse", scene.GetDirLight().getDiffuse() * scene.GetDirLight().getColor());
    lightPassShader->setUniform("parallelLight.specular", scene.GetDirLight().getSpecular() * scene.GetDirLight().getColor());
    lightPassShader->setUniform("parallelLight.direction", scene.GetDirLight().getLightDir());
    lightPassShader->setUniform("parallelLight.enabled", input.isParallelLightOn());
    lightPassShader->setUniform("lightSpaceMatrix", LightSpaceMatrix);
    lightPassShader->setUniform("parallelShadows", parallelShadows);

    if (input.isParallelLightOn())
    {
        pointShadows = false;

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, parallelShadowFrameBuffer.getDepth2D());

        lightPassShader->setUniform("depthMap", 5);
    }
    else pointShadows = true;

    //point light
    lightPassShader->setUniform("pointShadows", pointShadows);

    for (int i = 0; i < scene.GetPointLights().size(); i++)
    {
        std::string base = "pointLight[" + std::to_string(i) + "]";

        lightPassShader->setUniform(base + ".ambient", scene.GetPointLights()[i].getAmbient() * scene.GetPointLights()[i].getColor());
        lightPassShader->setUniform(base + ".diffuse", scene.GetPointLights()[i].getDiffuse() * scene.GetPointLights()[i].getColor());
        lightPassShader->setUniform(base + ".specular", scene.GetPointLights()[i].getSpecular() * scene.GetPointLights()[i].getColor());
        lightPassShader->setUniform(base + ".position", scene.GetPointLights()[i].getLightPos());
        lightPassShader->setUniform(base + ".enabled", scene.GetPointLights()[i].lightOn() && input.isPointLightOn());
        lightPassShader->setUniform("far_plane", scene.GetPointLights()[i].getFar());

        if (input.isPointLightOn())
        {
            glActiveTexture(GL_TEXTURE6 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowFramebuffers[i]->getDepthCube());

            lightPassShader->setUniform("shadowMap[" + std::to_string(i) + "]", 6 + i);
        }
    }
    // screen quad draw
    screenQuad->draw();
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lightPassFrameBuffer.getFBO());
    glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(),
                      0, 0, window.getWidth(), window.getHeight(),
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, lightPassFrameBuffer.getFBO());
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // lightCube draw
    lightShader->use();

    // transform matrix
    lightShader->setUniform("view", camera.getViewMatrix());
    lightShader->setUniform("projection", camera.getProjectionMatrix());

    for (int i = 0; i < scene.GetPointLights().size(); i++)
    {
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(model, scene.GetPointLights()[i].getLightPos());
        lightModel = glm::scale(lightModel, glm::vec3(0.25f));
        lightShader->setUniform("model", lightModel);
        lightShader->setUniform("lightColor", scene.GetPointLights()[i].getColor());
        lightShader->setUniform("enabled", scene.GetPointLights()[i].lightOn() && input.isPointLightOn());
        if (scene.GetPointLights()[i].lightOn() && input.isPointLightOn())
        {
            cube->draw();
        }
    }

    glDisable(GL_CULL_FACE); // or glCullFace(GL_FRONT)
    auto& env = scene.GetEnvironment();

    backgroundShader->use();
    backgroundShader->setUniform("view", camera.getViewMatrix());
    backgroundShader->setUniform("projection", camera.getProjectionMatrix());
    backgroundShader->setUniform("usePost", usePostProcess);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.maps.envCubemap);
    cube->draw();

    glEnable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    int debugH = window.getHeight() / 4;
    int debugW = debugH * static_cast<int>(camera.aspect);

    //Shader& debugShader = *shaders[10];
    gbufferDebugShader->use();
    glBindVertexArray(screenQuad->getVAO());

    auto drawDebug = [&](GLuint tex, int index)
        {
            glViewport(
                0,                  // x
                window.getHeight() - (index + 1) * debugH,  // y£¨The OpenGL origin of the coordinate axis is in the lower left corner.£©
                debugW,
                debugH
            );

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            gbufferDebugShader->setUniform("debugTex", 0);
            gbufferDebugShader->setUniform("u_DebugMode", index);
            gbufferDebugShader->setUniform("fov", camera.getFov());
            gbufferDebugShader->setUniform("nearPlane", camera.getNearPlane());
            gbufferDebugShader->setUniform("farPlane", camera.getFarPlane());
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        };

    drawDebug(gPosition, 0);
    drawDebug(gNormal, 1);
    drawDebug(gAlbedoSpec, 2);
    drawDebug(gDepth, 3);

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::drawMesh(const Mesh& mesh, Shader& shader) const
{

    GLuint slot = 6; // 0 = depthMap, 1-4 = shadowMap

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
            case TextureType::ARM:
                uniformName = "arm";
                break;
        }
        //std::cout << "Binding Uniform: " << uniformName << " to Slot: " << slot << std::endl;
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
    FrameBuffer& hdrFrameBuffer = *framebuffers[0];
    FrameBuffer& gFrameBuffer = *framebuffers[2];
    FrameBuffer& lightPassFrameBuffer = *framebuffers[3];

    hdrFrameBuffer.resize(window.getWidth(), window.getHeight());
    gFrameBuffer.resize(window.getWidth(), window.getHeight());
    lightPassFrameBuffer.resize(window.getWidth(), window.getHeight());
    pingpongFrameBuffer[0]->resize(window.getWidth(), window.getHeight());
    pingpongFrameBuffer[1]->resize(window.getWidth(), window.getHeight());

    camera.aspect = (float)window.getWidth() / (float)window.getHeight();
}

void Renderer::onImGuiRender()
{
    ImGui::Begin("Post Processing");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    //dirLight.dirOnImGuiRender();
    for (int i = 0; i < scene.GetPointLights().size(); ++i)
    {
        scene.GetPointLights()[i].pointOnImGuiRender(i);
    }

    scene.GetDirLight().dirOnImGuiRender();

    ImGui::Combo("Effect Mode", &effectMode, "normal\0inversion\0grayscale\0sharpen\0blur\0\0");
    if (effectMode == 3 || effectMode == 4)
    {
        ImGui::SliderFloat("Offset", &offset, 100.0f, 1000.0f);
    }
    //ImGui::SliderFloat("Model Light", &modelLight, 0.1f, 1.0f);

    ImGui::SliderFloat("aoBias", &aoBias, -1.0f, 1.0f);
    ImGui::SliderFloat("roughnessBias", &roughnessBias, -1.0f, 1.0f);
    ImGui::SliderFloat("metallicBias", &metallicBias, -1.0f, 1.0f);
    ImGui::SliderFloat("Scan Pos", &scanPos, 0.0f, static_cast<float>(window.getWidth()));
    input.onImGuiRender();

    //ImGui::SameLine();
    
    ImGui::Checkbox("useNormal", &hasNormal);
    
    /*ImGui::SliderFloat("height_scale", &height_scale, 0.0005f, 0.01f);
    ImGui::SameLine();
    ImGui::Checkbox("useHeight", &hasHeight);*/
    
    ImGui::SliderFloat("Exposure", &exposure, 0.01f, 10.0f);
    ImGui::SameLine();
    ImGui::Checkbox("useHdr", &useHdr);
    //ImGui::SameLine();
    //ImGui::Checkbox("useGamma", &useGamma);
    
    ImGui::SliderFloat("samplerDistance", &samplerDistance, 0.01f, 10.0f);
    ImGui::SameLine();
    ImGui::Checkbox("useBloom", &useBloom);

    ImGui::Checkbox("useDeferred", &useDeferred);
    ImGui::SameLine();
    ImGui::Checkbox("usePost", &usePostProcess);
    ImGui::SameLine();
    ImGui::Checkbox("useShadow", &useShadows);

    ImGui::Checkbox("drawLights", &drawLights);
    ImGui::SameLine();
    ImGui::Checkbox("drawPlane", &drawPlane);

    scene.GetObjects()[0].transform.onImGuiRender();

    ImGui::End();
}