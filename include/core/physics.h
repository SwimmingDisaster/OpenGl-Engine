#pragma once
#include "mypch.h"
#include "ecs/entity.h"
#include "core/physxSimulationFilter.h"

class PhysicsManager
{
public:
	static PxFoundation *mFoundation;
	static PxPhysics *mPhysics;
	static PxCooking* mCooking;
	static PxScene *mScene;
	static float mAccumulator;
	static float mStepSize;
	static PhysxSimulatorCallback simulationCallback;

	static std::array<int, 32> collisionLayerMask;
	static std::array<int, 32> notifyLayerMask;


//	std::unordered_map<PxActor, std::shared_ptr<Entity>> mapi;
public:
	static void InitPhysx();
	static void ShutdownPhysx();
	static void Start();
	static void Update();
	static void SetupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 collisionMask, PxU32 notifyMask);

	static void AdvanceSimulation(float deltaTime);
};
