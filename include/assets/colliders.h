#pragma once
#include "mypch.h"
#include "assets/colliderBase.h"
#include "ecs/other/componentDefines.h"

//--------------BoxCollider--------------
class BoxCollider : public ColliderBase
{
	REGISTER(BoxCollider);

public:
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};

public:
	void Show() override;
	void Serialize(YAML::Emitter &out) override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;

	BoxCollider();
#ifdef SHOW_DELETED
public:
	virtual ~BoxCollider();
#else
public:
	virtual ~BoxCollider(){};
#endif
};

//--------------BoxCollider--------------
class SphereCollider : public ColliderBase
{
	REGISTER(SphereCollider);

public:
	float radius = 5.0f;

public:
	void Show() override;
	void Serialize(YAML::Emitter &out) override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;

	SphereCollider();
#ifdef SHOW_DELETED
public:
	virtual ~SphereCollider();
#else
public:
	virtual ~SphereCollider(){};
#endif
};

