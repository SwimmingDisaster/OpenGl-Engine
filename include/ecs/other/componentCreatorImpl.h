#pragma once
#include "mypch.h"
#include "ecs/entity.h"

#include "ecs/other/componentCreator.h"

template <class T>
class CreatorImpl : public Creator
{
public:
	CreatorImpl<T>(const std::string& classname) : Creator(classname) {}
	virtual ~CreatorImpl<T>() {}

	virtual std::shared_ptr<Component> create(std::shared_ptr<Entity>& entityRef) {
		return entityRef->AddComponentR<T>();
	}
};


