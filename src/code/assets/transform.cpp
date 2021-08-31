#include "mypch.h"
#include "assets/transform.h"


REGISTERIMPL(Transform);
GETNAMEIMPL(Transform);

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

[[nodiscard]] glm::mat4 Transform::GetTransform() {
	//constexpr glm::mat4 identity = glm::mat4(1.0f);
	//return glm::translate(identity, position) * glm::toMat4(glm::quat(glm::radians(rotation))) * glm::scale(identity, scale);

	if(!isCacheValid){
		cachedTransform = glm::mat4(1.0f);
		cachedTransform = glm::translate(cachedTransform, position);
		cachedTransform *= glm::toMat4(glm::quat(glm::radians(rotation)));
		cachedTransform = glm::scale(cachedTransform, scale);
		isCacheValid = true;
	}

	return cachedTransform;
}
[[nodiscard]] glm::mat4 Transform::GetTransformWithNoScale() {
	constexpr glm::mat4 identity = glm::mat4(1.0f);
	return glm::translate(identity, position) * glm::toMat4(glm::quat(glm::radians(rotation)));
}

[[nodiscard]] const glm::vec3& Transform::GetPosition() const {
	return position;
}
[[nodiscard]] const glm::vec3& Transform::GetRotation() const {
	return rotation;
}
[[nodiscard]] const glm::vec3& Transform::GetScale() const {
	return scale;
}

void Transform::SetPosition(glm::vec3 other) {
	isCacheValid = false;
	position = other;
}
void Transform::SetRotation(glm::vec3 other) {
	isCacheValid = false;
	rotation = other;
}
void Transform::SetScale(glm::vec3 other) {
	isCacheValid = false;
	scale = other;
}

void Transform::SetRotationX(float other){
	isCacheValid = false;
	rotation.x = other;
}
void Transform::SetRotationY(float other){
	isCacheValid = false;
	rotation.y = other;
}
void Transform::SetRotationZ(float other){
	isCacheValid = false;
	rotation.z = other;
}

#ifdef SHOW_DELETED
Transform::~Transform() {
	Log("Deleted " << name);
}
#endif
