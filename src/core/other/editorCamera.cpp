
#include "mypch.h"
#include "core/other/editorCamera.h"
#include "core/application.h"
#include "core/input.h"





EditorCamera::EditorCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : vFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY)
{
    vPos = position;
    vWorldUp = up;
    vRot.y = yaw;
    vRot.x = pitch;
    updateCameraVectors();
}


EditorCamera::EditorCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : vFront(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY)
{
    vPos = glm::vec3(posX, posY, posZ);
    vWorldUp = glm::vec3(upX, upY, upZ);
    vRot.y = yaw;
    vRot.x = pitch;
    updateCameraVectors();
}

glm::mat4 EditorCamera::GetViewMatrix()
{
    return glm::lookAt(vPos, vPos + vFront, vUp);
}

void EditorCamera::ProcessKeyboard(float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    float speedup = 1.0f;
    if (glfwGetKey(Application::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
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

void EditorCamera::ProcessMouseMovement(GLboolean constrainPitch)
{

    if (!isLocked) {

        vRot.y   += Input::Get().xoffset * mouseSensitivity;
        vRot.x   += Input::Get().yoffset * mouseSensitivity;

        vRot.y = fmod(vRot.y, 360.0f);

        if (constrainPitch)
        {
            if (vRot.x > 89.0f)
                vRot.x = 89.0f;
            if (vRot.x < -89.0f)
                vRot.x = -89.0f;
        }

        updateCameraVectors();
    }
}


void EditorCamera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(vRot.y)) * cos(glm::radians(vRot.x));
    front.y = sin(glm::radians(vRot.x));
    front.z = sin(glm::radians(vRot.y)) * cos(glm::radians(vRot.x));

    vFront = glm::normalize(front);


    vRight = glm::normalize(glm::cross(vFront, vWorldUp));
    vUp    = glm::normalize(glm::cross(vRight, vFront));
    vUp    = glm::rotate(vUp, glm::radians(0.0f), {0.0f, 0.0f, 1.0f});
    //vUp.z  = glm::radians(vRot.z);
}
