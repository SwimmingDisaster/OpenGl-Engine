#include "assets/lights.h"


REGISTERIMPL(PointLight);

void PointLight::Show()  {
    //	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
}
void PointLight::Serialize(YAML::Emitter& out) {
    out << YAML::Key << name;
    out << YAML::BeginMap;

	//	out << YAML::Key << "Position" << YAML::Value << position;

    out << YAML::EndMap;
}
void PointLight::Deserialize(const YAML::Node& data) {
    //	position = data["Position"].as<glm::vec3>();
}
void PointLight::Start(){

}
void PointLight::Update(){

}
void PointLight::Draw(){

}
void PointLight::End(){

}

#ifdef SHOW_DELETED
PointLight::~PointLight() {
    Log("Deleted " << name);
}
#endif
PointLight::PointLight() {
    name = "PointLight";
}
