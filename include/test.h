#pragma once
#include "mypch.h"
#include "component.h"
#include "transform.h"
#include "entity.h"

class TestComponent : public Component
{
public:
	std::shared_ptr<Transform> transform;
	float num = 0;
public:
	virtual void Start() {
		transform = m_parentEntity->GetComponent<Transform>();
		transform->position =  {0.0, 0.0f, 0.0f};
	}

	virtual void Show() {
		ImGui::DragFloat("Num", &num);
	}

	~TestComponent() {
		Log("Deleted TestComponent");
	}

	TestComponent() {
		m_name = "TestComponent";
	}
};