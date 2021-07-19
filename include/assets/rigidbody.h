#pragma once
#include "ecs/other/componentDefines.h"
#include "mypch.h"
#include "assets/transform.h"
#include "assets/colliderBase.h"
#include "assets/colliders.h"

class Rigidbody : public Component {
	REGISTER(Rigidbody);
public:
	PxRigidDynamic* aDynamicActor;
	PxRigidStatic* aStaticActor;

	PxMaterial* mMaterial;
	std::shared_ptr<Transform> tc;
	std::shared_ptr<ColliderBase> cc;
	glm::vec3 size;
	bool isStatic = false;

	bool constrainPos = false;
	bool constrainRot = false;

	bool isSimulation = true;
	bool isTrigger = false;

public:
	void Start() override;
	void Show() override;
	void Update() override;
	void Serialize(YAML::Emitter& out) override;
	void Deserialize(const YAML::Node& data) override;
	void OnCollision(const std::shared_ptr<Entity>& other) override;

	Rigidbody();
#ifdef SHOW_DELETED
public:
	virtual ~Rigidbody();
#else
public:
	virtual ~Rigidbody();
#endif
};
