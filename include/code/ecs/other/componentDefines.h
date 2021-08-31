#pragma once


#include "ecs/other/componentCreatorImpl.h"
#define REGISTER(classname) \
private: \
	static const CreatorImpl<classname> creator; \
	static const std::string name

#define REGISTERIMPL(classname) \
	const CreatorImpl<classname> classname::creator(#classname); \
	const std::string classname::name(#classname)

#define GETNAME() \
public: \
	virtual const std::string& GetName() override;

#define GETNAMEIMPL(classname) \
	const std::string& classname::GetName(){\
		return name; \
	} \
