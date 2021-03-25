#pragma once
#include "mypch.h"
#include "ecs/component.h"

class Transform : public Component {
public:
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};

public:
	virtual void Show()  {
		ImGui::DragFloat3("position", glm::value_ptr(position), 0.05f);
		ImGui::DragFloat3("rotation", glm::value_ptr(rotation), 0.05f);
		ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.01f);
	}

	virtual void Serialize(YAML::Emitter& out) {
		out << YAML::Key << m_name;
		out << YAML::BeginMap;

		out << YAML::Key << "Position" << YAML::Value << position;
		out << YAML::Key << "Rotation" << YAML::Value << rotation;
		out << YAML::Key << "Scale" << YAML::Value << scale;

		out << YAML::EndMap;
	}

	~Transform() {
		Log("Deleted " << m_name);
	}
	Transform() {
		m_name = "Transform";
	}

};