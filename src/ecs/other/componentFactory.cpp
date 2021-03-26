#include "mypch.h"


#include "ecs/entity.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentCreator.h"


void Factory::registerit(const std::string& classname, Creator* creator)
{
	get_table()[classname] = creator;
}

std::shared_ptr<Component> Factory::create(const std::string& classname, std::shared_ptr<Entity>& entityRef)
{
	std::unordered_map<std::string, Creator*>::iterator i;
	i = get_table().find(classname);

	if (i != get_table().end())
		return i->second->create(entityRef);
	else
		return nullptr;
}

std::unordered_map<std::string, Creator*>& Factory::get_table()
{
	static std::unordered_map<std::string, Creator*> table;
	return table;
}





