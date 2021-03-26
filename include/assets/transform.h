#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"

class Transform : public Component {
	REGISTER(Transform);
public:
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};

public:
	virtual void Show();
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(const YAML::Node& data);

	~Transform();
	Transform();

};