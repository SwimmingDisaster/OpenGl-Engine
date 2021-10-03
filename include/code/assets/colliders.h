#pragma once
#include "mypch.h"
#include "assets/colliderBase.h"
#include "ecs/other/componentDefines.h"


class Transform;
namespace YAML { class Emitter; }
namespace YAML { class Node; }
namespace physx { class PxTriangleMesh; }

//--------------BoxCollider--------------
class BoxCollider : public ColliderBase{
	REGISTER(BoxCollider);
	GETNAME();

public:
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};

public:
	void Show() override;
	void Serialize(YAML::Emitter &out) const override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;
};

//--------------BoxCollider--------------
class SphereCollider : public ColliderBase{
	REGISTER(SphereCollider);
	GETNAME();

public:
	float radius = 5.0f;

public:
	void Show() override;
	void Serialize(YAML::Emitter &out) const override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;

};

//--------------ConcaveMeshCollider--------------
class ConcaveMeshCollider : public ColliderBase{
	REGISTER(ConcaveMeshCollider);
	GETNAME();

private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	PxTriangleMesh* aTriangleMesh = nullptr;

	std::shared_ptr<Transform> transform;

public:
	std::string filePath;

public:
	void Show() override;
	void Start() override;
	void Serialize(YAML::Emitter &out) const override;
	void Deserialize(const YAML::Node &data) override;

	PxGeometryHolder GetGeometry() override;

	virtual ~ConcaveMeshCollider();
};
