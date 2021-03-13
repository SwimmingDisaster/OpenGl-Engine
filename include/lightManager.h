#pragma once
#include "shader.h"

class LightManager {
public:
	LightManager();
	void Update(Shader& shader);
	void Init();
	~LightManager();
public:
	unsigned int PointLightUBO = 0, DirLightUBO = 0, SpotLightUBO = 0;


};