#pragma once

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

std::ostream& operator<<(std::ostream& out, const glm::vec3& g);
std::ostream& operator<<(std::ostream& out, const glm::vec4& g);
std::ostream& operator<<(std::ostream& out, const glm::mat4& g);

#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)

#define Log(x) 		std::cout << "Log: " 	<< x << '\n'
#define Warn(x) 	std::cout << "Warn: " 	<< x << '\n'
#define Error(x)	std::cerr << "Error: " 	<< x << '\n'
#define ErrorAtPos(x)	std::cerr << "Error: " 	<< x << " In file: " << __FILE__ << " on line: " << __LINE__ << '\n'
#define ReturnIfNotZero(x)	\
int COMBINE(returnValue,__LINE__) = x; \
if (COMBINE(returnValue,__LINE__) != 0) {\
		ErrorAtPos(COMBINE(returnValue,__LINE__));\
		return COMBINE(returnValue,__LINE__);\
}\

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// the code is equal to this with added line number to the end of returnValue:
// int returnValue = x;
// if (returnValue != 0) {
//	 ErrorAtPos(returnValue);
//	 return returnValue;
// }
