#pragma once
#include "ecs/other/componentDefines.h"
#include "ecs/entity.h"
#include "assets/transform.h"

class TestComponent : public Component {
	REGISTER(TestComponent);

public:
	std::shared_ptr<Entity> entity;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Transform> otherTransform;
	float speed = 1.0f;
	std::string entity_name = "";
	glm::vec3 offset;

public:
	void Show() override;
	void Start() override;
	void Update() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;


	TestComponent();
#ifdef SHOW_DELETED
public:
	virtual ~TestComponent();
#else
public:
	virtual ~TestComponent() {};
#endif
};
