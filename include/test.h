#pragma once
#include "component.h"
#include "transform.h"
#include "entity.h"

class TestComponent : public Component
{
public:
	std::shared_ptr<Transform> transform;

public:
	virtual void Start() {
		transform = m_parentEntity->GetComponent<Transform>();
		transform->position =  {0.0, 999.9f, 69.0f};
		Log(transform->position);
	}
};