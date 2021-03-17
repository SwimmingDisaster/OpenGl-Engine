#pragma once
#include "mypch.h"



class Entity;
class Component {
public:
	virtual void Start() {}
	virtual void Update() {}
	virtual void End() {}
	virtual void Serialize() {}
	virtual void Deserialize() {}

	~Component();
public:
	std::shared_ptr<Entity> m_parentEntity;
	//+ GetComponent<template T>()
};