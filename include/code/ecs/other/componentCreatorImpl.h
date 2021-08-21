#pragma once
#include "mypch.h"
#include "ecs/entity.h"
#include "ecs/component.h"


#include "ecs/other/componentCreator.h"


template <class T>
class CreatorImpl : public Creator
{
public:
	CreatorImpl(const std::string& classname) noexcept : Creator(classname) {}

	virtual ~CreatorImpl() {}

	Component* create(Entity* entityRef) override {
		/*
		auto comp = entityRef->GetComponent<T>();
		if (comp == nullptr)
			return entityRef->AddComponent<T>();
		else
			return comp;
		*/

		return entityRef->GetComponentOrMakeIfNone<T>(); 
	}

	void copy(Entity* const entityRef, const Component* const componentRef) override {
		*entityRef->GetComponentOrMakeIfNone<T>() = *static_cast<const T*>(componentRef);
	}
};


