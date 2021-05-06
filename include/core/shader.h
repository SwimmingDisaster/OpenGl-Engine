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

	void setBool(const char* name, bool value) const noexcept;
	void setInt(const char* name, int value) const noexcept;
	void setFloat(const char* name, float value) const noexcept;
	void setVec2(const char* name, const glm::vec2 &value) const noexcept;
	void setVec2(const char* name, float x, float y) const noexcept;
	void setVec3(const char* name, const glm::vec3 &value) const noexcept;
	void setVec3(const char* name, float x, float y, float z) const noexcept;
	void setVec4(const char* name, const glm::vec4 &value) const noexcept;
	void setVec4(const char* name, const glm::vec3 &value) const noexcept;
	void setVec4(const char* name, float x, float y, float z, float w) const noexcept;
	void setMat2(const char* name, const glm::mat2 &mat) const noexcept;
	void setMat3(const char* name, const glm::mat3 &mat) const noexcept;
	void setMat4(const char* name, const glm::mat4 &mat) const noexcept;

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	bool operator== (Shader &rhs);
};