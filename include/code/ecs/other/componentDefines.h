#pragma once


#include "ecs/other/componentCreatorImpl.h"
#define REGISTER(classname) \
	private: \
	static const CreatorImpl<classname> creator

#define REGISTERIMPL(classname) \
	const CreatorImpl<classname> classname::creator(#classname)
