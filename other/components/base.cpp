#include "assets/ABCDEFG.h"


REGISTERIMPL(COMPONENTNAME);

void COMPONENTNAME::Show()  {
    //	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
}
void COMPONENTNAME::Serialize(YAML::Emitter& out) {
    out << YAML::Key << name;
    out << YAML::BeginMap;

	//	out << YAML::Key << "Position" << YAML::Value << position;

    out << YAML::EndMap;
}
void COMPONENTNAME::Deserialize(const YAML::Node& data) {
    //	position = data["Position"].as<glm::vec3>();
}
void COMPONENTNAME::Start(){

}
void COMPONENTNAME::Update(){

}
void COMPONENTNAME::Draw(){

}
void COMPONENTNAME::End(){

}

#ifdef SHOW_DELETED
COMPONENTNAME::~COMPONENTNAME() {
    Log("Deleted " << name);
}
#endif
COMPONENTNAME::COMPONENTNAME() {
    name = "COMPONENTNAME";
}
