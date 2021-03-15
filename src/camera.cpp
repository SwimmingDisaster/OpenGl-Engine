
#include "mypch.h"
#include "camera.h"
#include "game.h"
#include "input.h"





Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : vFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY)
{
    vPos = position;
    vWorldUp = up;
    fYaw = yaw;
    fPitch = pitch;
    updateCameraVectors();
}


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : vFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY)
{
    vPos = glm::vec3(posX, posY, posZ);
    vWorldUp = glm::vec3(upX, upY, upZ);
    fYaw = yaw;
    fPitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(vPos, vPos + vFront, vUp);
}

void Camera::ProcessKeyboard(float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    float speedup = 1.0f;
    if (glfwGetKey(Game::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speedup = 3.0f;

    if (Input::IsKeyHeld(INPUT_KEY_W)) {
        vPos += vFront * velocity * speedup;
    }
    if (Input::IsKeyHeld(INPUT_KEY_S)) {
        vPos -= vFront * velocity * speedup;
    }
    if (Input::IsKeyHeld(INPUT_KEY_A)) {
        vPos -= vRight * velocity * speedup;
    }
    if (Input::IsKeyHeld(INPUT_KEY_D)) {
        vPos += vRight * velocity * speedup;
    }

}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{

    if (!isLocked) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        fYaw   += xoffset;
        fPitch += yoffset;

        if (constrainPitch)
        {
            if (fPitch > 89.0f)
                fPitch = 89.0f;
            if (fPitch < -89.0f)
                fPitch = -89.0f;
        }

        updateCameraVectors();
    }
}


void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
    front.y = sin(glm::radians(fPitch));
    front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
    vFront = glm::normalize(front);

    vRight = glm::normalize(glm::cross(vFront, vWorldUp));
    vUp    = glm::normalize(glm::cross(vRight, vFront));
}
