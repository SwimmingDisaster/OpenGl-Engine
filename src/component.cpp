#include "component.h"
#include "log.h"

Component::~Component() {
	Log("deleted component");
}