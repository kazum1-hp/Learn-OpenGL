#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera
{
public:
    Camera();

    float aspect = 1920.0f / 1080.0f;
    
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    void setSensitivity(float s) { sensitivity = s; }
    glm::vec3 getPosition() const { return cameraPos; }
    glm::mat4 getViewMatrix() const {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
    }

private:

    void updateCameraVectors();

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;
    float fov;

    float sensitivity = 0.04f;
};
