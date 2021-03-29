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

	virtual std::shared_ptr<Component> create(std::shared_ptr<Entity>& entityRef) {
		return entityRef->AddComponentR<T>();
	}

	virtual void copy(std::shared_ptr<Entity>& entityRef, std::shared_ptr<Component>& componentRef) {
		/*		std::shared_ptr<T> ramb = std::make_shared<T>(*std::dynamic_pointer_cast<T>(componentRef));
				if (ramb != nullptr) {
					std::shared_ptr<T> a = entityRef->GetComponent<T>();
					*a = *ramb;
				}*/

		*entityRef->GetComponent<T>() = *std::make_shared<T>(*std::dynamic_pointer_cast<T>(componentRef));
		/*
				auto a = std::make_shared<T>(*std::dynamic_pointer_cast<T>(componentRef));
				auto b = entityRef->GetComponentR<T>();
				Log("okay this works");
				b = a;*/

	}
};


