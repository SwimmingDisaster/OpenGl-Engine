#pragma once
#include "mypch.h"

#include "ecs/other/componentDefines.h"
#include "ecs/component.h"               // for Component

class Camera;
class Transform;
namespace YAML { class Emitter; }
namespace YAML { class Node; }

class CameraFPSController : public Component {
	REGISTER(CameraFPSController);
	GETNAME();
public:
	Transform* transform;
	Camera* camera;
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

#ifdef SHOW_DELETED
public:
	virtual ~CameraFPSController();
#else
//public:
	//virtual ~CameraFPSController() {};
#endif

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(bool constrainPitch);
};


