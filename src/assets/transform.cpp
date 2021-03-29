#include "assets/transform.h"
#include "core/application.h"


REGISTERIMPL(Transform);

void Transform::Show()  {
	ImGui::DragFloat3("position", glm::value_ptr(position), 0.05f);
	ImGui::DragFloat3("rotation", glm::value_ptr(rotation), 0.05f);
	ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.01f);
}

void Transform::Serialize(YAML::Emitter& out) {
	out << YAML::Key << m_name;
	out << YAML::BeginMap;

	out << YAML::Key << "Position" << YAML::Value << position;
	out << YAML::Key << "Rotation" << YAML::Value << rotation;
	out << YAML::Key << "Scale" << YAML::Value << scale;

	out << YAML::EndMap;
}

void Transform::Deserialize(const YAML::Node& data) {
	position = data["Position"].as<glm::vec3>();
	rotation = data["Rotation"].as<glm::vec3>();
	scale = data["Scale"].as<glm::vec3>();
}

void Transform::Update() {
}


#ifdef SHOW_DELETED
Transform::~Transform() {
	Log("Deleted " << m_name);
}
#endif
Transform::Transform() {
	m_name = "Transform";
}