#pragma once

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& out, const glm::vec3& g);
std::ostream& operator<<(std::ostream& out, const glm::vec4& g);
std::ostream& operator<<(std::ostream& out, const glm::mat4& g);

#define Log(x) 		std::cout << "Log: " 	<< x << std::endl
#define Warn(x) 	std::cout << "Warn: " 	<< x << std::endl
#define Error(x)	std::cout << "Error: " 	<< x << std::endl