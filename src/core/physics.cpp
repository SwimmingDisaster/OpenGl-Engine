#include "mypch.h"
#include "core/physics.h"
#include "core/application.h"

PxFoundation* PhysicsManager::mFoundation;
PxPhysics* PhysicsManager::mPhysics;
//PxCooking* PhysicsManager::mCooking;
PxScene* PhysicsManager::mScene;


void PhysicsManager::InitPhysx() {

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!mFoundation)
		ErrorAtPos("PxCreateFoundation failed!");



	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
	                           PxTolerancesScale(), true, nullptr);
	if (!mPhysics)
		ErrorAtPos("PxCreatePhysics failed!");

	PxTolerancesScale scale;
	scale.length = 100;
	scale.speed = 981;


	/*	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(scale));
		if (!mCooking)
			ErrorAtPos("PxCreateCooking failed! In file: " << __FILE__ << " on line: " << __LINE__);*/


	if (!PxInitExtensions(*mPhysics, nullptr))
		ErrorAtPos("PxInitExtensions failed!");


	PxTolerancesScale tolerance = mPhysics->getTolerancesScale();
	PxSceneDesc sceneDesc(tolerance);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.broadPhaseType = PxBroadPhaseType::eABP;
	sceneDesc.flags = PxSceneFlag::eENABLE_ENHANCED_DETERMINISM;


	auto gDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	mScene = mPhysics->createScene(sceneDesc);
	if (!mScene)
		Error("createScene failed!");
}

void PhysicsManager::ShutdownPhysx() {
	PxCloseExtensions();
	if (mScene)
	{
		mScene->release();
	}
	//mCooking->release();
	mPhysics->release();
	mFoundation->release();
}

void PhysicsManager::Start() {

}

void PhysicsManager::Update() {
#ifndef RELEASE_BUILD //physics manager
	if (Application::isRunning) {
#endif
		Application::advance(EngineInfo::deltaTime);
#ifndef RELEASE_BUILD
	}
#endif
}
