//
// Created by Steve Wheeler on 29/09/2023.
//

#include "Camera.hpp"
#include "slib.hpp"

namespace sage
{

void Camera::Update(float deltaTime)
{
    view = slib::fpsviewGl(pos, rotation.x, rotation.y);
    forward = glm::vec3({view[0][2], view[1][2], view[2][2]});
    right = glm::vec3({view[0][0], view[1][0], view[2][0]});
    const float speed = 7.5f; // Adjust this value as needed
    const float adjustedSpeed = speed * deltaTime;

    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_W]) {
        pos -= forward * adjustedSpeed;
    }
    if (keyState[SDL_SCANCODE_S]) {
        pos += forward * adjustedSpeed;
    }
    if (keyState[SDL_SCANCODE_A]) {
        pos -= right * adjustedSpeed;
    }
    if (keyState[SDL_SCANCODE_D]) {
        pos += right * adjustedSpeed;
    }
}

void Camera::HandleEvent(SDL_Event *event)
{
    if (event->type == SDL_MOUSEMOTION) {
        rotate(event->motion.xrel, event->motion.yrel);
    }
}

void Camera::rotate(float x, float y)
{
    const float sensitivity = 0.075f;
    rotation.y -= x * sensitivity;
    rotation.x -= y * sensitivity;
}

Camera::Camera(glm::vec3 _pos,
               glm::vec3 _rotation,
               glm::vec3 _direction,
               glm::vec3 _up,
               float _fov,
               float _zFar,
               float _zNear) :
               pos(_pos), 
               rotation(_rotation), 
               direction(_direction), 
               up(_up), 
               fov(_fov), 
               zFar(_zFar), 
               zNear(_zNear)
{

}
}