#pragma once
#include "mypch.h"

class Entity;
class Creator;
class Component;

class Factory
{
public:
	static Component* create(const std::string& classname, Entity* entityRef);
	static void copy(const std::string& classname, Entity* entityRef, const Component* componentRef);
	static void registerit(const std::string& classname, Creator* creator);
	static std::unordered_map<std::string, Creator*>& get_table();
};

