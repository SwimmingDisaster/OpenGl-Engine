#include "mypch.h"
#include "core/physics.h"
#include "core/application.h"

PxFoundation *PhysicsManager::mFoundation;
PxPhysics *PhysicsManager::mPhysics;
//PxCooking* PhysicsManager::mCooking;
PxScene *PhysicsManager::mScene;
PhysxSimulatorCallback PhysicsManager::simulationCallback;

float PhysicsManager::mAccumulator = 0.0f;
float PhysicsManager::mStepSize = 1.0f / 60.0f;

std::array<int, 32> PhysicsManager::collisionLayerMask;
std::array<int, 32> PhysicsManager::notifyLayerMask;

void PhysicsManager::InitPhysx()
{
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
	sceneDesc.filterShader = SampleSubmarineFilterShader; 

	mScene = mPhysics->createScene(sceneDesc);
	if (!mScene)
		Error("createScene failed!");
	mScene->setSimulationEventCallback(&simulationCallback);
}

void PhysicsManager::ShutdownPhysx()
{
	PxCloseExtensions();
	if (mScene)
	{
		mScene->release();
	}
	//mCooking->release();
	mPhysics->release();
	mFoundation->release();
}

void PhysicsManager::Start()
{
}

void PhysicsManager::Update()
{
#ifndef RELEASE_BUILD //physics manager
	if (Application::isRunning)
	{
#endif
		AdvanceSimulation(EngineInfo::deltaTime);
#ifndef RELEASE_BUILD
	}
#endif
}
void PhysicsManager::SetupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 collisionMask, PxU32 notifyMask)
{
    PxFilterData filterData;
    filterData.word0 = filterGroup; 
    filterData.word1 = collisionMask;  
    filterData.word2 = notifyMask;  
    const PxU32 numShapes = actor->getNbShapes();
    PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*)*numShapes);
    actor->getShapes(shapes, numShapes);
    for(PxU32 i = 0; i < numShapes; i++)
    {
        PxShape* shape = shapes[i];
        shape->setSimulationFilterData(filterData);
    }
    free(shapes);
}

void PhysicsManager::AdvanceSimulation(float deltaTime)
{
	mAccumulator += deltaTime;
	if (mAccumulator < mStepSize)
		return; // false;

	while (mAccumulator >= mStepSize)
	{
		mAccumulator -= mStepSize;
		mScene->simulate(mStepSize);
		mScene->fetchResults(true);
	}
	//return true;
}
