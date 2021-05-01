#include "mypch.h"
#include "assets/rigidbody.h"
#include "core/physics.h"
#include "core/application.h"
#include "assets/transform.h"
#include "ecs/entity.h"

REGISTERIMPL(Rigidbody);

void Rigidbody::Show()
{
	ImGui::Checkbox("Is Static", &isStatic);
}

void Rigidbody::Serialize(YAML::Emitter &out)
{
	out << YAML::Key << name;
	out << YAML::BeginMap;

	out << YAML::Key << "Is Static" << isStatic;

	out << YAML::EndMap;
}

void Rigidbody::Deserialize(const YAML::Node &data)
{
	isStatic = data["Is Static"].as<bool>();
}

void Rigidbody::Start()
{

	tc = parentEntity->GetComponent<Transform>();
	cc = parentEntity->GetComponent<ColliderBase>();
	size = tc->scale;

	mMaterial = PhysicsManager::mPhysics->createMaterial(10.0f, 10.0f, 0.1f); //static friction, dynamic friction, restitution
	if (!mMaterial)
		Error("createMaterial failed!");

	glm::mat4 glmTransform = tc->GetTransformWithNoScale();
	PxMat44 physxTransform;
	GlmMat4ToPhysXMat4(glmTransform, physxTransform);

	if (!isStatic)
		aDynamicActor = PxCreateDynamic(*PhysicsManager::mPhysics, PxTransform(physxTransform), cc->GetGeometry().any(), *mMaterial, 1.0f);
	else
		aStaticActor = PxCreateStatic(*PhysicsManager::mPhysics, PxTransform(physxTransform), cc->GetGeometry().any(), *mMaterial);
	if (!aDynamicActor || !aStaticActor)
		Error("create shape failed!");

	if (!isStatic)
		PhysicsManager::mScene->addActor(*aDynamicActor);
	else
	{
		PhysicsManager::mScene->addActor(*aStaticActor);
	}
}
void Rigidbody::Update()
{
	//aSphereActor->setGlobalPose({tc->position.x, tc->position.y, tc->position.z});
	if (!isStatic)
	{
		if (!aDynamicActor->isSleeping())
		{
			PxTransform transform = aDynamicActor->getGlobalPose();
			PxMat44 transformMat = PxMat44(transform);
			glm::mat4 newMat;

			glm::vec3 translation;
			glm::vec3 rotation;
			glm::vec3 scale;
			PhysXMat4ToglmMat4(transformMat, newMat);
			DecomposeTransform(newMat, translation, rotation, scale);

			auto pxvec3Vel = aDynamicActor->getLinearVelocity();
			tc->position = translation + glm::vec3(pxvec3Vel.x, pxvec3Vel.y, pxvec3Vel.z) * PhysicsManager::mAccumulator;
			tc->rotation = glm::degrees(rotation);
			tc->scale = scale;
			tc->scale = size;
		}
		else
		{
			parentEntity->GetComponent<Material>()->color = glm::vec3(0.0f, 255.0f, 0.0f);
		}
	}
}

#ifdef SHOW_DELETED
Rigidbody::~Rigidbody()
{
	Log("Deleted " << name);
	aSphereActor->release();
	mMaterial->release();
}
#else
Rigidbody::~Rigidbody()
{
	if (!isStatic)
		aDynamicActor->release();
	else
		aStaticActor->release();
	mMaterial->release();
}
#endif
Rigidbody::Rigidbody()
{
	name = "Rigidbody";
}