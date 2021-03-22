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
		ImGui::DragFloat3("position", glm::value_ptr(position));
		ImGui::DragFloat3("rotation", glm::value_ptr(rotation));
		ImGui::DragFloat3("scale", glm::value_ptr(scale));
	}

	~Transform() {
		Log("Deleted " << m_name);
	}
	Transform() {
		m_name = "Transform";
	}

};