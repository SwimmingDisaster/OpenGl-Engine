#pragma once
#include "component.h"
#include <string.h>


class Transform : public Component {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

public:
	virtual void Start();
	virtual void Show();
	~Transform();
	Transform();
};