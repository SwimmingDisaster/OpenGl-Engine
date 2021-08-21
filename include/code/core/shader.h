#pragma once

#include "mypch.h"

class Shader: public std::enable_shared_from_this<Shader> {
public:
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;
	static std::vector<std::shared_ptr<Shader>> shaderList;
	static std::vector<std::string> shaderNames;
	static int BoundShaderID;

private:
	unsigned int ID = 0;
	std::unordered_map<std::string, GLint> uniform_cache;

public:
	Shader();
	~Shader();

	[[nodiscard]] const unsigned int& GetID() const;

	void CreateVertexAndFragment(const std::string& name);
	void CreateCompute(const char* computePath);

	void use();

	void setBool(const char* name, bool value) noexcept;
	void setInt(const char* name, int value) noexcept;
	void setFloat(const char* name, float value) noexcept;
	void setVec2(const char* name, const glm::vec2 &value) noexcept;
	void setVec2(const char* name, float x, float y) noexcept;
	void setVec3(const char* name, const glm::vec3 &value) noexcept;
	void setVec3(const char* name, float x, float y, float z) noexcept;
	void setVec4(const char* name, const glm::vec4 &value) noexcept;
	void setVec4(const char* name, const glm::vec3 &value) noexcept;
	void setVec4(const char* name, float x, float y, float z, float w) noexcept;
	void setMat2(const char* name, const glm::mat2 &mat) noexcept;
	void setMat3(const char* name, const glm::mat3 &mat) noexcept;
	void setMat4(const char* name, const glm::mat4 &mat) noexcept;

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	bool operator==(Shader &rhs);

	[[nodiscard]] GLint GetUniformLocation(const char* name);
};
