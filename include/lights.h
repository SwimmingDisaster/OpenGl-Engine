#pragma once
#include <glm/glm.hpp>


struct DirLight {
	glm::vec4 direction_junk;

	glm::vec4 ambient_junk;
	glm::vec4 diffuse_junk;
	glm::vec4 specular_junk;
	glm::vec4 intensities;
};

struct PointLight {
	glm::vec4 position_junk;
	glm::vec4 ambient_constant;
	glm::vec4 diffuse_linear;
	glm::vec4 specular_quadratic;
	glm::vec4 intensities;
};

struct SpotLight {
	glm::vec4 position_cutOff;
	glm::vec4 direction_outerCutOff;

	glm::vec4 ambient_constant;
	glm::vec4 diffuse_linear;
	glm::vec4 specular_quadratic;
	glm::vec4 intensities;
};