#pragma once
#include <vector>
#include <memory>
#include "Model.h"
#include "Light.h"
#include "Skybox.h"
#include "Transform.h"
#include "Material.h"

// 为了能在场景中放置模型，我们需要一个结构体来保存 模型指针 + 位置信息
struct RenderObject {
    std::shared_ptr<Model> model;
    Transform transform;
    // 如果需要覆盖模型默认材质，可以在这里加 Material
    std::shared_ptr<Material> material;
};

struct EnvironmentData {
    GLuint hdrTexture;        // 原始 HDR 纹理
    GLuint envCubemap;       // 转换后的立方体环境贴图
    GLuint irradianceMap;    // 漫反射卷积图
    GLuint prefilterMap;     // 镜面反射预过滤图
    GLuint brdfLUT;          // BRDF 查找表
};

class Scene {
public:
    // 默认初始化一个方向光
    Scene() :dirLight(glm::vec3(1.0f), 1.0f, glm::vec3(-2.2f, -2.0f, -2.3f), LightType::Directional) {}

    // --- 添加物体 ---
    void AddObject(std::shared_ptr<Model> model, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), std::shared_ptr<Material> mat = nullptr) {
        RenderObject obj;
        obj.model = model;
        obj.transform.setPosition(pos);
        obj.transform.setScale(scale);
        obj.material = mat;
        objects.push_back(obj);
    }

    // --- 灯光管理 ---
    void AddPointLight(const Light& light) {
        pointLights.push_back(light);
    }

    // --- Getters ---
    const std::vector<RenderObject>& GetObjects() const { return objects; }
    std::vector<RenderObject>& GetObjects() { return objects; }
    const std::vector<Light>& GetPointLights() const { return pointLights; }
    std::vector<Light>& GetPointLights() { return pointLights; }
    const Light& GetDirLight() const { return dirLight; }
    Light& GetDirLight() { return dirLight; }

    //void SetSkybox(std::shared_ptr<Skybox> sb) { skybox = sb; }
    //const Skybox* GetSkybox() const { return skybox.get(); }
    void SetSkybox(GLuint hdrTexID) { env.hdrTexture = hdrTexID; }
    const GLuint GetSkybox() const { return env.hdrTexture; }

private:
    std::vector<RenderObject> objects;
    std::vector<Light> pointLights;
    Light dirLight;
    std::shared_ptr<Skybox> skybox;
    EnvironmentData env;
};
