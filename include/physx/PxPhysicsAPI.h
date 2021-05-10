//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2019 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.


#ifndef PX_PHYSICS_NXPHYSICS_API
#define PX_PHYSICS_NXPHYSICS_API
/** \addtogroup physics
@{
*/

/**
This is the main include header for the Physics SDK, for users who
want to use a single #include file.

Alternatively, one can instead directly #include a subset of the below files.
*/

// Foundation SDK
#include "pxshared/foundation/Px.h"
#include "pxshared/foundation/PxAllocatorCallback.h"
#include "physx/foundation/PxAssert.h"
#include "pxshared/foundation/PxBitAndData.h"
#include "pxshared/foundation/PxBounds3.h"
#include "pxshared/foundation/PxErrorCallback.h"
#include "pxshared/foundation/PxErrors.h"
#include "pxshared/foundation/PxFlags.h"
#include "pxshared/foundation/PxIntrinsics.h"
#include "pxshared/foundation/PxIO.h"
#include "pxshared/foundation/PxMat33.h"
#include "pxshared/foundation/PxMat44.h"
#include "pxshared/foundation/PxMath.h"
#include "physx/foundation/PxMathUtils.h"
#include "pxshared/foundation/PxPlane.h"
#include "pxshared/foundation/PxPreprocessor.h"
#include "pxshared/foundation/PxQuat.h"
#include "pxshared/foundation/PxSimpleTypes.h"
#include "pxshared/foundation/PxStrideIterator.h"
#include "pxshared/foundation/PxTransform.h"
#include "pxshared/foundation/PxUnionCast.h"
#include "pxshared/foundation/PxVec2.h"
#include "pxshared/foundation/PxVec3.h"
#include "pxshared/foundation/PxVec4.h"

//Not physics specific utilities and common code
#include "physx/common/PxCoreUtilityTypes.h"
#include "physx/common/PxPhysXCommonConfig.h"
#include "physx/common/PxRenderBuffer.h"
#include "physx/common/PxBase.h"
#include "physx/common/PxTolerancesScale.h"
#include "physx/common/PxTypeInfo.h"
#include "physx/common/PxStringTable.h"
#include "physx/common/PxSerializer.h"
#include "physx/common/PxMetaData.h"
#include "physx/common/PxMetaDataFlags.h"
#include "physx/common/PxSerialFramework.h"
#include "physx/common/PxPhysicsInsertionCallback.h"

//Task Manager
#include "physx/task/PxTask.h"

// Cuda Mananger
#if PX_SUPPORT_GPU_PHYSX
#include "physx/gpu/PxGpu.h"
#endif

//Geometry Library
#include "physx/geometry/PxBoxGeometry.h"
#include "physx/geometry/PxBVHStructure.h"
#include "physx/geometry/PxCapsuleGeometry.h"
#include "physx/geometry/PxConvexMesh.h"
#include "physx/geometry/PxConvexMeshGeometry.h"
#include "physx/geometry/PxGeometry.h"
#include "physx/geometry/PxGeometryHelpers.h"
#include "physx/geometry/PxGeometryQuery.h"
#include "physx/geometry/PxHeightField.h"
#include "physx/geometry/PxHeightFieldDesc.h"
#include "physx/geometry/PxHeightFieldFlag.h"
#include "physx/geometry/PxHeightFieldGeometry.h"
#include "physx/geometry/PxHeightFieldSample.h"
#include "physx/geometry/PxMeshQuery.h"
#include "physx/geometry/PxMeshScale.h"
#include "physx/geometry/PxPlaneGeometry.h"
#include "physx/geometry/PxSimpleTriangleMesh.h"
#include "physx/geometry/PxSphereGeometry.h"
#include "physx/geometry/PxTriangle.h"
#include "physx/geometry/PxTriangleMesh.h"
#include "physx/geometry/PxTriangleMeshGeometry.h"


// PhysX Core SDK
#include "physx/PxActor.h"
#include "physx/PxAggregate.h"
#include "physx/PxArticulation.h"
#include "physx/PxArticulationReducedCoordinate.h"
#include "physx/PxArticulationJoint.h"
#include "physx/PxArticulationJointReducedCoordinate.h"
#include "physx/PxArticulationLink.h"
#include "physx/PxBatchQuery.h"
#include "physx/PxBatchQueryDesc.h"
#include "physx/PxClient.h"
#include "physx/PxConstraint.h"
#include "physx/PxConstraintDesc.h"
#include "physx/PxContact.h"
#include "physx/PxContactModifyCallback.h"
#include "physx/PxDeletionListener.h"
#include "physx/PxFiltering.h"
#include "physx/PxForceMode.h"
#include "physx/PxFoundation.h"
#include "physx/PxLockedData.h"
#include "physx/PxMaterial.h"
#include "physx/PxPhysics.h"
#include "physx/PxPhysicsVersion.h"
#include "physx/PxPhysXConfig.h"
#include "physx/PxQueryFiltering.h"
#include "physx/PxQueryReport.h"
#include "physx/PxRigidActor.h"
#include "physx/PxRigidBody.h"
#include "physx/PxRigidDynamic.h"
#include "physx/PxRigidStatic.h"
#include "physx/PxScene.h"
#include "physx/PxSceneDesc.h"
#include "physx/PxSceneLock.h"
#include "physx/PxShape.h"
#include "physx/PxSimulationEventCallback.h"
#include "physx/PxSimulationStatistics.h"
#include "physx/PxVisualizationParameter.h"
#include "physx/PxPruningStructure.h"

//Character Controller
#include "physx/characterkinematic/PxBoxController.h"
#include "physx/characterkinematic/PxCapsuleController.h"
#include "physx/characterkinematic/PxController.h"
#include "physx/characterkinematic/PxControllerBehavior.h"
#include "physx/characterkinematic/PxControllerManager.h"
#include "physx/characterkinematic/PxControllerObstacles.h"
#include "physx/characterkinematic/PxExtended.h"

//Cooking (data preprocessing)
#include "physx/cooking/Pxc.h"
#include "physx/cooking/PxConvexMeshDesc.h"
#include "physx/cooking/PxCooking.h"
#include "physx/cooking/PxTriangleMeshDesc.h"
#include "physx/cooking/PxBVH33MidphaseDesc.h"
#include "physx/cooking/PxBVH34MidphaseDesc.h"
#include "physx/cooking/PxMidphaseDesc.h"

//Extensions to the SDK
#include "physx/extensions/PxDefaultStreams.h"
#include "physx/extensions/PxDistanceJoint.h"
#include "physx/extensions/PxExtensionsAPI.h"
#include "physx/extensions/PxFixedJoint.h"
#include "physx/extensions/PxJoint.h"
#include "physx/extensions/PxJointLimit.h"
#include "physx/extensions/PxPrismaticJoint.h"
#include "physx/extensions/PxRevoluteJoint.h"
#include "physx/extensions/PxRigidBodyExt.h"
#include "physx/extensions/PxShapeExt.h"
#include "physx/extensions/PxSimpleFactory.h"
#include "physx/extensions/PxSmoothNormals.h"
#include "physx/extensions/PxSphericalJoint.h"
#include "physx/extensions/PxStringTableExt.h"
#include "physx/extensions/PxTriangleMeshExt.h"
#include "physx/extensions/PxConvexMeshExt.h"

//Serialization
#include "physx/extensions/PxSerialization.h"
#include "physx/extensions/PxBinaryConverter.h"
#include "physx/extensions/PxRepXSerializer.h"

//Vehicle Simulation
#include "physx/vehicle/PxVehicleComponents.h"
#include "physx/vehicle/PxVehicleDrive.h"
#include "physx/vehicle/PxVehicleDrive4W.h"
#include "physx/vehicle/PxVehicleDriveTank.h"
#include "physx/vehicle/PxVehicleSDK.h"
#include "physx/vehicle/PxVehicleShaders.h"
#include "physx/vehicle/PxVehicleTireFriction.h"
#include "physx/vehicle/PxVehicleUpdate.h"
#include "physx/vehicle/PxVehicleUtilControl.h"
#include "physx/vehicle/PxVehicleUtilSetup.h"
#include "physx/vehicle/PxVehicleUtilTelemetry.h"
#include "physx/vehicle/PxVehicleWheels.h"
#include "physx/vehicle/PxVehicleNoDrive.h"
#include "physx/vehicle/PxVehicleDriveNW.h"

//Connecting the SDK to Visual Debugger
#include "physx/pvd/PxPvdSceneClient.h"
#include "physx/pvd/PxPvd.h"
#include "physx/pvd/PxPvdTransport.h"
/** @} */
#endif
