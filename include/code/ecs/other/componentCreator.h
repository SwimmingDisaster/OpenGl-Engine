#pragma once
#include "mypch.h"

class Component;
class Entity;

class Creator
{
public:
	Creator(const std::string& classname) noexcept;
	virtual ~Creator() {};

	virtual Component* create(Entity* const entityRef) = 0;

	virtual void copy(Entity* const entityRef, const Component* const componentRef) = 0;
};

