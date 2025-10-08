#include "Camera.h"

Camera camera; // 定义全局摄像机

Camera::Camera()
    : cameraPos(0.0f, 0.0f, 3.0f),
    cameraFront(0.0f, 0.0f, -1.0f),
    cameraUp(0.0f, 1.0f, 0.0f),
    yaw(-90.f),
    pitch(0.0f),
    fov(45.0f)
{
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(f);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    float sensitivity = 0.04f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    fov -= yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 75.0f) fov = 75.0f;
}

glm::mat4 Camera::cameraView() const
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::MoveForward(float speed)
{
    cameraPos += cameraFront * speed;
}

void Camera::MoveBackward(float speed)
{
    cameraPos -= cameraFront * speed;
}

void Camera::MoveLeft(float speed)
{
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

void Camera::MoveRight(float speed)
{
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}
