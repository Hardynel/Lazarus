#include "camera.h"

Camera::Camera(glm::vec3 position,
               glm::vec3 front, 
               glm::vec3 world_up,
               float yaw, 
               float pitch,
               float last_x,
               float last_y,
               float movement_speed,
               float sensitivity,
               float fov)
               : position(position)
               , front(front)
               , world_up(world_up)
               , yaw(yaw)
               , pitch(pitch)
               , last_x(last_x)
               , last_y(last_y)
               , movement_speed(movement_speed)
               , sensitivity(sensitivity)
               , fov(fov)
{
    update_camera_vectors();
}

void Camera::process_camera_rotate(float x_offset, float y_offset)
{
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    update_camera_vectors();
}

void Camera::update_camera_vectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front  = glm::normalize(front);
    right  = glm::normalize(glm::cross(front, world_up));
    up     = glm::normalize(glm::cross(right, front));
    
}

void Camera::process_camera_zoom(double x_offset, double y_offset)
{
    fov -= static_cast<float>(y_offset);

    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}

void Camera::process_keyboard_input(MovementKey key, float delta_time)
{
    float velocity = movement_speed * delta_time;
    
    // movement on the xz plane
    if (key == MovementKey::W)
    {
        position.x += front.x * velocity;
        position.z += front.z * velocity;
    }
    if (key == MovementKey::S)
    {
        position.x -= front.x * velocity;
        position.z -= front.z * velocity; 
    }
    if (key == MovementKey::A)
    {
        position.x -= right.x * velocity;
        position.z -= right.z * velocity; 
    }
    if (key == MovementKey::D)
    {
        position.x += right.x * velocity;
        position.z += right.z * velocity;
    }
}

glm::mat4 Camera::look_at(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& world_up)
{
    glm::vec3 front = glm::normalize(position - direction);
    glm::vec3 right = glm::normalize(glm::cross(glm::normalize(world_up), front));
    glm::vec3 up = glm::cross(front, right);

    glm::mat4 rotation = glm::mat4(1.0f);
    
    rotation[0][0] = right.x;
    rotation[1][0] = right.y;
    rotation[2][0] = right.z;
    rotation[0][1] = up.x;
    rotation[1][1] = up.y;
    rotation[2][1] = up.z;
    rotation[0][2] = front.x;
    rotation[1][2] = front.y;
    rotation[2][2] = front.z;
    
    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;

    return rotation * translation;
}

glm::mat4 Camera::get_view_matrix()
{
    //return glm::lookAt(position_, position_ + front_, up_);
    return look_at(position, position + front, world_up);
}
