#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;

    float fov;

    Camera();
    glm::mat4 cameraView() const;

    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:

    void updateCameraVectors();

};

// 声明全局摄像机
extern Camera camera;