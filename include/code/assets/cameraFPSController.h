#pragma once
#include "mypch.h"

#include "ecs/other/componentDefines.h"
#include "assets/transform.h"
#include "assets/camera.h"

class CameraFPSController : public Component {
	REGISTER(CameraFPSController);
public:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Camera> camera;
	float mouseSensitivity = 0.35f;
	float movementSpeed = 15.0f;
	bool isLocked = false;
	glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};
public:
	void Start() override;
	void Update() override;
	void Show() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;

	CameraFPSController();
#ifdef SHOW_DELETED
public:
	virtual ~CameraFPSController();
#else
public:
	virtual ~CameraFPSController() {};
#endif

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(bool constrainPitch);
};


