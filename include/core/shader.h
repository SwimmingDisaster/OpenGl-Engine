#pragma once

#include "mypch.h"

class Shader: public std::enable_shared_from_this<Shader> {
public:
	static std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
	unsigned int ID;

public:
	Shader();
	~Shader();

	void CreateVertexAndFragment(const char* vertexPath, const char* fragmentPath);
	void CreateCompute(const char* computePath);

	void use();

	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec2(const char* name, const glm::vec2 &value) const;
	void setVec2(const char* name, float x, float y) const;
	void setVec3(const char* name, const glm::vec3 &value) const;
	void setVec3(const char* name, float x, float y, float z) const;
	void setVec4(const char* name, const glm::vec4 &value) const;
	void setVec4(const char* name, const glm::vec3 &value) const;
	void setVec4(const char* name, float x, float y, float z, float w) const;
	void setMat2(const char* name, const glm::mat2 &mat) const;
	void setMat3(const char* name, const glm::mat3 &mat) const;
	void setMat4(const char* name, const glm::mat4 &mat) const;

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	bool operator== (Shader &rhs);
};