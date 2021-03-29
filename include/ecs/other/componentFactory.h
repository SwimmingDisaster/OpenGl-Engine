#pragma once
#include "mypch.h"
#include "ecs/other/componentCreatorImpl.h"
#include "ecs/entity.h"


class Creator;

class Factory
{
public:
	static std::shared_ptr<Component> create(const std::string& classname, std::shared_ptr<Entity>& entityRef);
	static void copy(const std::string& classname, std::shared_ptr<Entity>& entityRef, std::shared_ptr<Component>& componentRef);
	static void registerit(const std::string& classname, Creator* creator);
	static std::unordered_map<std::string, Creator*>& get_table();
};

#define REGISTER(classname) \
	private: \
	static const CreatorImpl<classname> creator

#define REGISTERIMPL(classname) \
	const CreatorImpl<classname> classname::creator(#classname)


