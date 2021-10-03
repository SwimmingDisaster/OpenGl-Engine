#pragma once
#include "ecs/other/componentDefines.h"
#include "ecs/entity.h"
#include "assets/transform.h"

class TestComponent : public Component {
	REGISTER(TestComponent);
	GETNAME();
public:
	Entity* entity;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Transform> otherTransform;
	float speed = 1.0f;
	std::string entity_name = "";
	glm::vec3 offset = {0.0f, 0.0f, 0.0f};

public:
	void Show() override;
	void Start() override;
	void Update() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;
};
