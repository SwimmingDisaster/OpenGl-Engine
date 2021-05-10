#pragma once
#include "mypch.h"
#include "ecs/entity.h"
#include "ecs/component.h"


#include "ecs/other/componentCreator.h"


template <class T>
class CreatorImpl : public Creator
{
public:
	CreatorImpl<T>(const std::string& classname) : Creator(classname) {}
	virtual ~CreatorImpl<T>() {}

	std::shared_ptr<Component> create(std::shared_ptr<Entity>& entityRef) override {
		auto comp = entityRef->GetComponent<T>();
		if (comp == nullptr)
			return entityRef->AddComponentR<T>();
		else
			return comp;
	}

	void copy(const std::shared_ptr<Entity>& entityRef, std::shared_ptr<Component>& componentRef) override {
		*entityRef->GetComponentOrMakeIfNone<T>() = *std::make_shared<T>(*std::dynamic_pointer_cast<T>(componentRef));
	}
};


