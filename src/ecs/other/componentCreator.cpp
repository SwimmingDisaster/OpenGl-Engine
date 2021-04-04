#include "mypch.h"

#include "ecs/other/componentCreator.h"
#include "ecs/other/componentFactory.h"

Creator::Creator(const std::string& classname)
{
	Factory::registerit(classname, this);
	int kelp;

}
