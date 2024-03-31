#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class MovementKey
{
    W,
    S,
    A,
    D
};

const float YAW = -90.0f;
const float PITCH = 0.0f;

const float LAST_X = 800.0f / 2.0f;
const float LAST_Y = 600.0f / 2.0f;
const float MOVEMENT_SPEED = 2.5f;
const float SENSITIVITY = 0.01f;
const float FOV = 45.0f;

class Camera
{ 
public:
    // Euler's angles
    float yaw_;
    float pitch_;

    // mouse rotation and zoom
    float last_x_;
    float last_y_;
    float movement_speed_;
    float sensitivity_;
    float fov_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 world_up_;
    glm::vec3 right_;
    glm::vec3 up_;

    Camera(glm::vec3 position       = glm::vec3(0.0f, 0.0f,  3.0f),
           glm::vec3 front          = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 world_up       = glm::vec3(0.0f, 1.0f,  0.0f),
           float yaw = YAW,
           float pitch = PITCH,
           float last_x = LAST_X,
           float last_y = LAST_Y,
           float movement_speed = MOVEMENT_SPEED,
           float sensitivity = SENSITIVITY,
           float fov = FOV
    );

    void process_camera_rotate(float x_pos, float y_pos);
    void process_camera_zoom(double x_offset, double y_offset);
    void process_keyboard_input(MovementKey key, float delta_time);
    glm::mat4 look_at(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& world_up);
    glm::mat4 get_view_matrix();

private:
    void update_camera_vectors();
};

#endif
