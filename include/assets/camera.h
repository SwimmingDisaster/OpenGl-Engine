#pragma once
#include "mypch.h"

#include "ecs/component.h"
#include "ecs/other/componentFactory.h"

#include "assets/transform.h"


//#define _ITERATOR_DEBUG_LEVEL 0


class Camera : public Component {
	REGISTER(Camera);
public:
	glm::vec3 vFront{};

	glm::vec3 vUp{};
	glm::vec3 vRight{};
	glm::vec3 vWorldUp = {0.0f, 1.0f, 0.0f};


	bool isPrimary = false;

	float fov = 90.0f;
	float fNear = 0.05f;
	float fFar = 1000.0f;

	glm::vec3 backgroundColor = {0.1f, 0.1f, 0.1f};

public:
	std::shared_ptr<Transform> transform;

public:
	void Start() override;
	void Update() override;
	void Show() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;


	Camera();
#ifdef SHOW_DELETED
public:
	virtual ~Camera();
#else
public:
	virtual ~Camera() {};
#endif

	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetProjectionMatrix() const;


	void updateCameraVectors();

};
