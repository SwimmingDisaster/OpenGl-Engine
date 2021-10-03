#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentDefines.h"

class Camera;
class Entity;
class Rigidbody;
class Transform;
namespace YAML { class Emitter; }
namespace YAML { class Node; }

class PhysicsFPScontroler : public Component {
	REGISTER(PhysicsFPScontroler);
	GETNAME();
public:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Rigidbody> rigidbody;
	float mouseSensitivity = 0.35f;
	float movementSpeed = 15.0f;
	bool isLocked = false;
    bool canjump = false;
	glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};

public:
    void Start() override;
    void Update() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;
	void OnCollision(const Entity* const other) override;

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(bool constrainPitch);
};
