#pragma once
#include "mypch.h"
#include "ecs/component.h"
//#include "ecs/entity.h"

class Entity;
class Creator
{
public:
	Creator(const std::string& classname) noexcept;
	virtual ~Creator() {};

	virtual std::shared_ptr<Component> create(std::shared_ptr<Entity>& entityRef) = 0;

	virtual void copy(const std::shared_ptr<Entity>& entityRef, std::shared_ptr<Component>& componentRef) = 0;
};

