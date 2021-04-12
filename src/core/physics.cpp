#include "mypch.h"
#include "core/physics.h"


PxFoundation* PhysicsManager::mFoundation;
PxPhysics* PhysicsManager::mPhysics;
//PxCooking* PhysicsManager::mCooking;
PxScene* PhysicsManager::mScene;


void PhysicsManager::InitPhysx() {

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (!mFoundation)
		ErrorAtPos("PxCreateFoundation failed! In file: " << __FILE__ << " on line: " << __LINE__);



	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
	                           PxTolerancesScale(), true, nullptr);
	if (!mPhysics)
		ErrorAtPos("PxCreatePhysics failed! In file: " << __FILE__ << " on line: " << __LINE__);

	PxTolerancesScale scale;
	scale.length = 100;        // typical length of an object
	scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice


	/*	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(scale));
		if (!mCooking)
			ErrorAtPos("PxCreateCooking failed! In file: " << __FILE__ << " on line: " << __LINE__);*/


	if (!PxInitExtensions(*mPhysics, nullptr))
		ErrorAtPos("PxInitExtensions failed! In file: " << __FILE__ << " on line: " << __LINE__);


	PxTolerancesScale tolerance = mPhysics->getTolerancesScale();
	PxSceneDesc sceneDesc(tolerance);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);


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

void Start() {

}

void Update() {

}
