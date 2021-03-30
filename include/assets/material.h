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
	virtual void Show();
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(const YAML::Node& data);

	Material();
#ifdef SHOW_DELETED
	~Material();
#endif
};