#pragma once
#include "mypch.h"

class PhysicsManager
{
public:
	static PxFoundation *mFoundation;
	static PxPhysics *mPhysics;
	//PxCooking* mCooking;
	static PxScene *mScene;
	static float mAccumulator;
	static float mStepSize;

public:
	static void InitPhysx();
	static void ShutdownPhysx();
	static void Start();
	static void Update();

	static void AdvanceSimulation(float deltaTime);
};