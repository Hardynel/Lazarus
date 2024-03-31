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
               : yaw_(yaw)
               , pitch_(pitch)
               , last_x_(last_x)
               , last_y_(last_y)
               , movement_speed_(movement_speed)
               , sensitivity_(sensitivity)
               , fov_(fov)
               , position_(position)
               , front_(front)
               , world_up_(world_up)
{
    update_camera_vectors();
}

void Camera::process_camera_rotate(float x_offset, float y_offset)
{
    x_offset *= sensitivity_;
    y_offset *= sensitivity_;

    yaw_ += x_offset;
    pitch_ += y_offset;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    update_camera_vectors();
}

void Camera::update_camera_vectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_  = glm::normalize(front);
    right_  = glm::normalize(glm::cross(front_, world_up_));
    up_     = glm::normalize(glm::cross(right_, front_));
    
}

void Camera::process_camera_zoom(double x_offset, double y_offset)
{
    fov_ -= static_cast<float>(y_offset);

    if (fov_ < 1.0f)
        fov_ = 1.0f;
    if (fov_ > 90.0f)
        fov_ = 90.0f;
}

void Camera::process_keyboard_input(MovementKey key, float delta_time)
{
    float velocity = movement_speed_ * delta_time;
    
    if (key == MovementKey::W)
    {
        position_ += front_ * velocity;
    }
    if (key == MovementKey::S)
    {
        position_ -= front_ * velocity;
    }
    if (key == MovementKey::A)
    {
        position_ -= right_ * velocity;
    }
    if (key == MovementKey::D)
    {
        position_ += right_ * velocity;
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
    return look_at(position_, position_ + front_, world_up_);
}
