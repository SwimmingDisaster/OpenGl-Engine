#pragma once

#include "glm/glm.hpp"
#include "pxshared/foundation/PxMat44.h"

using namespace physx;

bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

void PhysXMat4ToglmMat4(const PxMat44& in, glm::mat4& out);
void GlmMat4ToPhysXMat4(const glm::mat4& in, PxMat44& out);
