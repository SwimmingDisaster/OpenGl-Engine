#pragma once
#include "mypch.h"
#include "ecs/entity.h"

class PhysicsManager
{
public:
	static PxFoundation *mFoundation;
	static PxPhysics *mPhysics;
	//PxCooking* mCooking;
	static PxScene *mScene;
	static float mAccumulator;
	static float mStepSize;

//	std::unordered_map<PxActor, std::shared_ptr<Entity>> mapi;
public:
	static void InitPhysx();
	static void ShutdownPhysx();
	static void Start();
	static void Update();

	static void AdvanceSimulation(float deltaTime);
};
