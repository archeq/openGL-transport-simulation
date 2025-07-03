//
// Created by KUCIA on 22.06.2025.
//

#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float ratio) {
    this->position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    this->ratio = ratio;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float ratio) {
    position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    this->ratio = ratio;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_actions action, float deltaTime) {
    if (mode == FREE) {
        float velocity = MovementSpeed * deltaTime;
        if (action == FORWARD)
            position += Front * velocity;
        if (action == BACKWARD)
            position -= Front * velocity;
        if (action == LEFT)
            position -= Right * velocity;
        if (action == RIGHT)
            position += Right * velocity;
        if (action == UP)
            position += Up * velocity;
        if (action == DOWN)
            position -= Up * velocity;
        if (action == KEY_1)
            MovementSpeed = 10.0f;
        if (action == KEY_2)
            MovementSpeed = 15.0f;
        if (action == KEY_3)
            MovementSpeed = 25.0f;
    }
    if (mode == FOLLOW) {
        // In FOLLOW mode, the camera follows a target (e.g., a train)
    }

}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    if (mode == FREE) {
        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

    }

    if (mode == FOLLOW) {
        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Convert spherical coordinates to Cartesian coordinates
        float yawRad = glm::radians(Yaw);
        float pitchRad = glm::radians(Pitch);

        glm::vec3 offset;
        offset.x = follow_distance * cos(pitchRad) * cos(yawRad);
        offset.y = follow_distance * sin(pitchRad);
        offset.z = follow_distance * cos(pitchRad) * sin(yawRad);

        position = target_position - offset; // Keep the camera at the right distance, orbiting the target
        Front = glm::normalize(target_position - position); // Always look at the target

    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    if (mode == FREE) {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    if (mode == FOLLOW) {
        // In FOLLOW mode, the camera's Front vector is always directed towards the target
        // Convert spherical coordinates to Cartesian coordinates
        float yawRad = glm::radians(Yaw);
        float pitchRad = glm::radians(Pitch);

        glm::vec3 offset;
        offset.x = follow_distance * cos(pitchRad) * cos(yawRad);
        offset.y = follow_distance * sin(pitchRad);
        offset.z = follow_distance * cos(pitchRad) * sin(yawRad);

        position = target_position - offset; // Keep the camera at the right distance, orbiting the target
        Front = glm::normalize(target_position - position); // Always look at the target
        // Recalculate Right and Up vectors
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(Zoom), ratio, near_distance, far_distance);
}

void Camera::update() {
    if (mode == FOLLOW)
        updateCameraVectors();
}

