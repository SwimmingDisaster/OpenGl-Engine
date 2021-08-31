#include "mypch.h"
#include "assets/rigidbody.h"
#include "core/physics.h"
#include "core/application.h"
#include "assets/transform.h"
#include "ecs/entity.h"

REGISTERIMPL(Rigidbody);
GETNAMEIMPL(Rigidbody);

void Rigidbody::Show(){
    ImGui::Checkbox("Is Static", &isStatic);
    ImGui::Checkbox("Constrain position", &constrainPos);
    ImGui::Checkbox("Constrain rotation", &constrainRot);
    ImGui::Checkbox("Is simulation", &isSimulation);
    ImGui::Checkbox("Is trigger", &isTrigger);
}
void Rigidbody::Serialize(YAML::Emitter& out) const{
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "Is Static" << isStatic;
    out << YAML::Key << "Constrain position" << constrainPos;
    out << YAML::Key << "Constrain rotation" << constrainRot;
    out << YAML::Key << "Is simulation" << isSimulation;
    out << YAML::Key << "Is trigger" << isTrigger;

    out << YAML::EndMap;
}
void Rigidbody::Deserialize(const YAML::Node &data){
    isStatic = data["Is Static"].as<bool>();
    constrainPos = data["Constrain position"].as<bool>();
    constrainRot = data["Constrain rotation"].as<bool>();
    isSimulation = data["Is simulation"].as<bool>();
    isTrigger = data["Is trigger"].as<bool>();
}
void Rigidbody::Start(){
    tc = parentEntity->GetComponent<Transform>();
    cc = parentEntity->GetComponent<ColliderBase>();
    //size = tc->GetScale();

    mMaterial = PhysicsManager::mPhysics->createMaterial(10.0f, 10.0f, 0.1f); //static friction, dynamic friction, restitution
    if (mMaterial == nullptr) {
        Error("createMaterial failed!");
    }

    glm::mat4 glmTransform = tc->GetTransformWithNoScale();
    PxMat44 physxTransform;
    GlmMat4ToPhysXMat4(glmTransform, physxTransform);

    if (!isStatic) {
        aDynamicActor = PxCreateDynamic(*PhysicsManager::mPhysics, PxTransform(physxTransform), cc->GetGeometry().any(), *mMaterial, 1.0f);
    } else {
        aStaticActor = PxCreateStatic(*PhysicsManager::mPhysics, PxTransform(physxTransform), cc->GetGeometry().any(), *mMaterial);
    }
    if ((aDynamicActor == nullptr) && (aStaticActor == nullptr)) {
        Error("create actor failed!");
    }

    if(isStatic) {
        aStaticActor->setGlobalPose({tc->GetPosition().x, tc->GetPosition().y, tc->GetPosition().z});
    }

    if (!isStatic) {
        aDynamicActor->setName(parentEntity->GetUUIDString().c_str());
        PhysicsManager::mScene->addActor(*aDynamicActor);
    }
    else
    {
        aStaticActor->setName(parentEntity->GetUUIDString().c_str());
        PhysicsManager::mScene->addActor(*aStaticActor);
    }
    if(!isStatic) {
        PxShape* treasureShape = nullptr;
        aDynamicActor->getShapes(&treasureShape, 1);
        treasureShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, isSimulation);
        treasureShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
    }
    else {
        PxShape* treasureShape = nullptr;
        aStaticActor->getShapes(&treasureShape, 1);
        treasureShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, isSimulation);
        treasureShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
    }

    if(!isStatic) {
        const int& layer = parentEntity->GetLayer();
        PhysicsManager::SetupFiltering(aDynamicActor, 1UL << layer, PhysicsManager::collisionLayerMask[layer], PhysicsManager::notifyLayerMask[layer]);
    }
    else {
        const int& layer = parentEntity->GetLayer();
        PhysicsManager::SetupFiltering(aStaticActor, 1UL << layer, PhysicsManager::collisionLayerMask[layer], PhysicsManager::notifyLayerMask[layer]);
    }
}
void Rigidbody::Update(){
    //aSphereActor->setGlobalPose({tc->position.x, tc->position.y, tc->position.z});
    if (!isStatic)
    {
        if (!aDynamicActor->isSleeping())
        {
            PxTransform transform = aDynamicActor->getGlobalPose();
            auto transformMat = PxMat44(transform);
            glm::mat4 newMat;

            glm::vec3 translation;
            glm::vec3 rotation;
            glm::vec3 scale;
            PhysXMat4ToglmMat4(transformMat, newMat);
            DecomposeTransform(newMat, translation, rotation, scale);

            auto pxvec3Vel = aDynamicActor->getLinearVelocity();
            if(!constrainPos) {
                tc->SetPosition(translation + glm::vec3(pxvec3Vel.x, pxvec3Vel.y, pxvec3Vel.z) * PhysicsManager::mAccumulator);
            }
            if(!constrainRot) {
                tc->SetRotation(glm::degrees(rotation));
            }
            tc->SetScale(scale);
            //tc->scale = size;
        }
    }
}
void Rigidbody::OnCollision(const Entity* const other) {
    //Log(parentEntity->GetName() << " collided with " << other->GetName());
}

#ifdef SHOW_DELETED
Rigidbody::~Rigidbody(){
    Log("Deleted " << name);
    if (!isStatic)
        aDynamicActor->release();
    else
        aStaticActor->release();
    mMaterial->release();
}
#else
Rigidbody::~Rigidbody(){
    if (!isStatic) {
        aDynamicActor->release();
    } else {
        aStaticActor->release();
    }
    mMaterial->release();
}
#endif


