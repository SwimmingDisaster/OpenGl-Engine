#include "assets/transform.h"


REGISTERIMPL(Transform);

void Transform::Show()  {
    ImGui::DragFloat3("position", glm::value_ptr(position), 0.05f);
    ImGui::DragFloat3("rotation", glm::value_ptr(rotation), 0.05f);
    ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.01f);
}
void Transform::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
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

[[nodiscard]] const glm::mat4 Transform::GetTransform() const{
	//constexpr glm::mat4 identity = glm::mat4(1.0f);
	//return glm::translate(identity, position) * glm::toMat4(glm::quat(glm::radians(rotation))) * glm::scale(identity, scale);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, position);
	trans *= glm::toMat4(glm::quat(glm::radians(rotation)));
	trans = glm::scale(trans, scale);

	return trans;
}
[[nodiscard]] const glm::mat4 Transform::GetTransformWithNoScale() const{
	constexpr glm::mat4 identity = glm::mat4(1.0f);
	return glm::translate(identity, position)
	       * glm::toMat4(glm::quat(glm::radians(rotation)));
}

#ifdef SHOW_DELETED
Transform::~Transform() {
    Log("Deleted " << name);
}
#endif
Transform::Transform() {
    name = "Transform";
}
