#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/entity.h"

class Creator
{
public:
	Creator(const std::string& classname);
	virtual ~Creator() {};

	virtual std::shared_ptr<Component> create(std::shared_ptr<Entity>& entityRef) = 0;
};

