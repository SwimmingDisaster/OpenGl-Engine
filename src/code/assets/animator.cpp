/*
#include "assets/animator.h"


REGISTERIMPL(Animator);

void Animator::Show()  {
    //	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
}
void Animator::Serialize(YAML::Emitter& out) {
    out << YAML::Key << name;
    out << YAML::BeginMap;

	//	out << YAML::Key << "Position" << YAML::Value << position;

    out << YAML::EndMap;
}
void Animator::Deserialize(const YAML::Node& data) {
    //	position = data["Position"].as<glm::vec3>();
}
void Animator::Start(){

}
void Animator::Update(){

}
void Animator::Draw(){

}
void Animator::End(){

}

#ifdef SHOW_DELETED
Animator::~COMPONENTNAME() {
    Log("Deleted " << name);
}
#endif
Animator::COMPONENTNAME()
    :name("Animator") {}
	*/
