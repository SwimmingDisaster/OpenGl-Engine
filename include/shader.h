#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "lights.h"


class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* computePath);
	Shader();


	void use();

	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec2(const char* name, const glm::vec2 &value) const;
	void setVec2(const char* name, float x, float y) const;
	void setVec3(const char* name, const glm::vec3 &value) const;
	void setVec3(const char* name, float x, float y, float z) const;
	void setVec4(const char* name, const glm::vec4 &value) const;
	//void setVec4(const char* name, const glm::vec3 &value) const;
	void setVec4(const char* name, float x, float y, float z, float w) const;
	void setMat2(const char* name, const glm::mat2 &mat) const;
	void setMat3(const char* name, const glm::mat3 &mat) const;
	void setMat4(const char* name, const glm::mat4 &mat) const;

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
};