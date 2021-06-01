#pragma once
#include "assets/mesh.h"
#include "assets/material.h"

class BatchRenderer{
public:
	static void AddObject(std::unique_ptr<Mesh>, std::unique_ptr<Material>);
};
