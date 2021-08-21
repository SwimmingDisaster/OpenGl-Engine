#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

class Transform : public Component {
	REGISTER(Transform);
public:
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};


public:
	void Show() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;

	[[nodiscard]] const glm::mat4 GetTransform() const;
	[[nodiscard]] const glm::mat4 GetTransformWithNoScale() const;

	Transform();
#ifdef SHOW_DELETED
public:
	virtual ~Transform();
#else
//public:
	//virtual ~Transform() {};
#endif
};


