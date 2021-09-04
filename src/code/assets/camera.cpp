#include "mypch.h"
#include "assets/camera.h"

#include "core/renderer.h"
#include "assets/transform.h"
#include "core/other/helpers.h"          // for operator<<
#include "ecs/entity.h"                  // for Entity
#include "core/engineInfo.h"             // for EngineInfo


REGISTERIMPL(Camera);
GETNAMEIMPL(Camera);

#ifdef SHOW_DELETED
Camera::~Camera() {
    Log("Deleted " << name);
}
#endif


void Camera::Start() {
    transform = parentEntity->GetComponent<Transform>();
}
void Camera::Update() {
    updateCameraVectors();
    Renderer::viewMatrix = GetViewMatrix();
    Renderer::projectionMatrix = GetProjectionMatrix();
    Renderer::clearColor = backgroundColor;
	Renderer::viewPos = transform->GetPosition();
}
void Camera::Show() {
    ImGui::DragFloat("Fov", &fov, 0.1f);
    ImGui::DragFloat("fNear", &fNear, 0.001f);
    ImGui::DragFloat("fFar", &fFar, 0.1f);
    ImGui::ColorEdit3("Background Color", glm::value_ptr(backgroundColor));
}

void Camera::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;
    out << YAML::Key << "Fov" << YAML::Value << fov;
    out << YAML::Key << "fNear" << YAML::Value << fNear;
    out << YAML::Key << "fFar" << YAML::Value << fFar;
    out << YAML::Key << "Background Color" << YAML::Value << backgroundColor;
    out << YAML::EndMap;
}

void Camera::Deserialize(const YAML::Node& data) {
    fov = data["Fov"].as<float>();
    fNear = data["fNear"].as<float>();
    fFar = data["fFar"].as<float>();
    backgroundColor = data["Background Color"].as<glm::vec3>();
}

const glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(transform->GetPosition(), transform->GetPosition() + vFront, vUp);
}

const glm::mat4 Camera::GetProjectionMatrix() const
{
    float ratio = 1.0f;
    if (EngineInfo::SCREEN_HEIGHT != 0.0f) {
        ratio = (float)EngineInfo::SCREEN_WIDTH / (float)EngineInfo::SCREEN_HEIGHT;
    }
    return glm::perspective(glm::radians(fov), ratio, fNear, fFar);
}




void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(transform->GetRotation().y)) * cos(glm::radians(transform->GetRotation().x));
    front.y = sin(glm::radians(transform->GetRotation().x));
    front.z = sin(glm::radians(transform->GetRotation().y)) * cos(glm::radians(transform->GetRotation().x));

    vFront = glm::normalize(front);


    vRight = glm::normalize(glm::cross(vFront, vWorldUp));
    vUp    = glm::normalize(glm::cross(vRight, vFront));
    vUp    = glm::rotate(vUp, glm::radians(transform->GetRotation().z), vFront);
}
