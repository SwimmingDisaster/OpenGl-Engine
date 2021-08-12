#include "mypch.h"
#include "assets/cameraFPSController.h"

#include "core/input.h"
#include "core/application.h"

REGISTERIMPL(CameraFPSController);


void CameraFPSController::Start() {
    transform = parentEntity->GetComponent<Transform>();
    camera = parentEntity->GetComponent<Camera>();
    //transform->position.y = 2.0f;
}
void CameraFPSController::Update() {
    if (Input::IsKeyPressed(INPUT_KEY_ESCAPE)) {
        if (isLocked) {
            isLocked = false;
            glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            isLocked = true;
            glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }


    ProcessKeyboard(EngineInfo::deltaTime);
    ProcessMouseMovement(true);

}
void CameraFPSController::Show() {
    ImGui::DragFloat("Mouse Sensitivity", &mouseSensitivity, 0.1f);
    ImGui::DragFloat("Movement Speed", &movementSpeed, 0.001f);
    ImGui::Checkbox("Is Locked", &isLocked);
}
void CameraFPSController::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "Mouse Sensitivity" << YAML::Value << mouseSensitivity;
    out << YAML::Key << "Movement Speed" << YAML::Value << movementSpeed;

    out << YAML::EndMap;
}
void CameraFPSController::Deserialize(const YAML::Node& data) {
    mouseSensitivity = data["Mouse Sensitivity"].as<float>();
    movementSpeed = data["Movement Speed"].as<float>();
}


CameraFPSController::CameraFPSController() {
    name = "CameraFPSController";
}
#ifdef SHOW_DELETED
CameraFPSController::~CameraFPSController() {
    Log("Deleted " << name);
}
#endif


void CameraFPSController::ProcessKeyboard(float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    //static bool canjump = false;
    if (glfwGetKey(Application::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        velocity *= 3.0f;
    }
    glm::vec3 velvec3 = glm::vec3(velocity, velocity/*0*/, velocity);

    if (Input::IsKeyHeld(INPUT_KEY_W)) {
        transform->position += camera->vFront * velvec3;
    }
    if (Input::IsKeyHeld(INPUT_KEY_S)) {
        transform->position -= camera->vFront * velvec3;
    }
    if (Input::IsKeyHeld(INPUT_KEY_A)) {
        transform->position -= camera->vRight * velvec3;
    }
    if (Input::IsKeyHeld(INPUT_KEY_D)) {
        transform->position += camera->vRight * velvec3;
    }
    /*	if (Input::IsKeyHeld(INPUT_KEY_SPACE)) {
    		if (canjump) {
    			acceleration.y = 20.0f;
    			canjump = false;
    		}
    	}
    	acceleration.y -= 30.0f * deltaTime;
    	transform->position += acceleration * deltaTime;
    	if (transform->position.y < 2.0f) {
    		transform->position.y = 2.0f;
    		acceleration.y = 0.0f;
    		canjump = true;
    	}*/

    //acceleration.x -= 30.0f * deltaTime, 0;
    //acceleration.z -=  30.0f * deltaTime, 0;
}

void CameraFPSController::ProcessMouseMovement(bool constrainPitch)
{

    if (!isLocked) {

        transform->rotation.y   += Input::Get().xoffset * mouseSensitivity;
        transform->rotation.x   += Input::Get().yoffset * mouseSensitivity;

        transform->rotation.y = fmod(transform->rotation.y, 360.0f);

        if (constrainPitch)
        {
            if (transform->rotation.x > 89.0f) {
                transform->rotation.x = 89.0f;
            }
            if (transform->rotation.x < -89.0f) {
                transform->rotation.x = -89.0f;
            }
        }

        camera->updateCameraVectors();
    }
}
