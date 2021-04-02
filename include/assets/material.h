#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"


class Material : public Component {
	REGISTER(Material);
public:
	glm::vec3 color = {1.0f, 1.0f, 1.0f};
public:
	void Show() override ;
	void Serialize(YAML::Emitter& out) override;
	void Deserialize(const YAML::Node& data) override;

	Material();
#ifdef SHOW_DELETED
public:
	virtual ~Material();
#else
public:
	virtual ~Material() {};
#endif
};