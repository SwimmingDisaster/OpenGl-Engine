#include "assets/physicsFPScontroller.h"
#include "core/application.h"
#include "core/input.h"

REGISTERIMPL(PhysicsFPScontroler);

void PhysicsFPScontroler::Show()  {
    //	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
    ImGui::DragFloat("Mouse Sensitivity", &mouseSensitivity, 0.1f);
    ImGui::DragFloat("Movement Speed", &movementSpeed, 0.001f);
    ImGui::Checkbox("Is Locked", &isLocked);
}
void PhysicsFPScontroler::Serialize(YAML::Emitter& out) {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "Mouse Sensitivity" << YAML::Value << mouseSensitivity;
    out << YAML::Key << "Movement Speed" << YAML::Value << movementSpeed;

    out << YAML::EndMap;
}
void PhysicsFPScontroler::Deserialize(const YAML::Node& data) {
    //	position = data["Position"].as<glm::vec3>();
    mouseSensitivity = data["Mouse Sensitivity"].as<float>();
    movementSpeed = data["Movement Speed"].as<float>();
}
void PhysicsFPScontroler::Start() {
    transform = parentEntity->GetComponent<Transform>();
    camera = parentEntity->GetComponent<Camera>();
    rigidbody = parentEntity->GetComponent<Rigidbody>();
}
void PhysicsFPScontroler::Update() {
    if (Input::IsKeyPressed(INPUT_KEY_ESCAPE)) {
        if (isLocked) {
            isLocked = false;
            glfwSetInputMode(Application::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            isLocked = true;
            glfwSetInputMode(Application::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }


    ProcessKeyboard(EngineInfo::deltaTime);
    ProcessMouseMovement(true);

}

void PhysicsFPScontroler::ProcessKeyboard(float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    static bool canjump = false;
    if (glfwGetKey(Application::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        velocity *= 3.0f;
    glm::vec3 velvec3 = glm::vec3(velocity, 0, velocity);

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
    if (Input::IsKeyHeld(INPUT_KEY_SPACE)) {
        if (canjump) {
            acceleration.y = 20.0f;
            canjump = false;
        }
    }
    acceleration.y -= 30.0f * deltaTime;
    transform->position += acceleration * deltaTime;
    if (transform->position.y < 2.05f) {
        transform->position.y = 2.01f;
        acceleration.y = 0.0f;
        canjump = true;
    }

    if (rigidbody && Application::isRunning) {
        rigidbody->aDynamicActor->setGlobalPose({transform->position.x, transform->position.y, transform->position.z}, false);
		rigidbody->aDynamicActor->setLinearVelocity({0.0f, 0.0f, 0.0f});
    }
    //acceleration.x -= 30.0f * deltaTime, 0;
    //acceleration.z -=  30.0f * deltaTime, 0;
}

void PhysicsFPScontroler::ProcessMouseMovement(bool constrainPitch)
{

    if (!isLocked) {

        transform->rotation.y   += Input::Get().xoffset * mouseSensitivity;
        transform->rotation.x   += Input::Get().yoffset * mouseSensitivity;

        transform->rotation.y = fmod(transform->rotation.y, 360.0f);

        if (constrainPitch)
        {
            if (transform->rotation.x > 89.0f)
                transform->rotation.x = 89.0f;
            if (transform->rotation.x < -89.0f)
                transform->rotation.x = -89.0f;
        }

        camera->updateCameraVectors();
    }
}


#ifdef SHOW_DELETED
PhysicsFPScontroler::~physicsFPScontroler() {
    Log("Deleted " << name);
}
#endif
PhysicsFPScontroler::PhysicsFPScontroler() {
    name = "PhysicsFPScontroler";
}
