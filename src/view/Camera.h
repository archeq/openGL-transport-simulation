//
// Created by KUCIA on 22.06.2025.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"


enum Camera_actions {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    KEY_1,
    KEY_2,
    KEY_3,
};

enum Camera_modes {
    FREE,
    FOLLOW,
};

class Camera {
public:
    glm::vec3 position;
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed = 15.0f;
    float MouseSensitivity = 0.14f;
    float Zoom = 45.0f;
    float near_distance = 0.1f;
    float far_distance = 200.0f;
    float ratio;
    Camera_modes mode = FREE; // default camera mode'
    glm::vec3 target_position; // target to follow
    float follow_distance = 3.0f; // distance from the target in FOLLOW mode
    void update();

    // constructor with vectors
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0,
           float pitch = 0.0,
           float ratio = 16.0/9.0);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float ratio = 16.0/9.0);


    [[nodiscard]] glm::mat4 getViewMatrix() const; // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    void ProcessKeyboard(Camera_actions action, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    glm::mat4 getProjectionMatrix() const;

private:
    void updateCameraVectors();  // calculates the front vector from the Camera's (updated) Euler Angles

};


#endif //CAMERA_H
