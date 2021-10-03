#include "mypch.h"
#include "assets/deleteAfterTouched.h"
#include "core/engineInfo.h"
#include "ecs/entity.h"             // for Entity
#include "assets/material.h"        // for Material

namespace YAML { class Node; }

REGISTERIMPL(DeleteAfterTouched);
GETNAMEIMPL(DeleteAfterTouched);

void DeleteAfterTouched::Show()  {
    //	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
    //ImGui::Text(std::to_string(time).c_str());
}
void DeleteAfterTouched::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    //	out << YAML::Key << "Position" << YAML::Value << position;

    out << YAML::EndMap;
}
void DeleteAfterTouched::Deserialize(const YAML::Node& data) {
    //	position = data["Position"].as<glm::vec3>();
}
void DeleteAfterTouched::Start() {
    material = parentEntity->GetComponent<Material>();
}
void DeleteAfterTouched::Update() {
    if(isTouched) {
        time -= EngineInfo::deltaTime;
    }
    if(time > 2.0f) {
        material->SetProperty("color", glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (time > 0.5f) {
        material->SetProperty("color", glm::vec3(1.0f, 0.39f, 0.075f));
    }
    else {
        material->SetProperty("color", glm::vec3((1.0f - time) * 2.0f, 0.0f, 0.0f));
    }
}
void DeleteAfterTouched::OnCollision(const Entity* const other) {
    isTouched = true;
}
