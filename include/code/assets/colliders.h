#pragma once
#include "mypch.h"
#include "assets/colliderBase.h"
#include "ecs/other/componentDefines.h"

#include "assets/transform.h"

//--------------BoxCollider--------------
class BoxCollider : public ColliderBase{
	REGISTER(BoxCollider);

public:
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};

public:
	void Show() override;
	void Serialize(YAML::Emitter &out) const override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;

	BoxCollider();
#ifdef SHOW_DELETED
public:
	virtual ~BoxCollider();
#else
//public:
	//virtual ~BoxCollider(){};
#endif
};

//--------------BoxCollider--------------
class SphereCollider : public ColliderBase{
	REGISTER(SphereCollider);

public:
	float radius = 5.0f;

public:
	void Show() override;
	void Serialize(YAML::Emitter &out) const override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;

	SphereCollider();
#ifdef SHOW_DELETED
public:
	virtual ~SphereCollider();
#else
//public:
	//virtual ~SphereCollider(){};
#endif
};

//--------------ConcaveMeshCollider--------------
class ConcaveMeshCollider : public ColliderBase{
	REGISTER(ConcaveMeshCollider);

private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	PxTriangleMesh* aTriangleMesh = nullptr;

	Transform* transform;

public:
	std::string filePath;

public:
	void Show() override;
	void Start() override;
	void Serialize(YAML::Emitter &out) const override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;

	ConcaveMeshCollider();
#ifdef SHOW_DELETED
public:
	virtual ~ConcaveMeshCollider();
#else
public:
	virtual ~ConcaveMeshCollider();
#endif
};
