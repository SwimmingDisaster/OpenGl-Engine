#pragma once
#include "mypch.h"
#include "ecs/other/componentDefines.h"

class ColliderBase : public Component {
public:
	virtual PxGeometryHolder GetGeometry() = 0;
};