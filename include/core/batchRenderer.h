#pragma once
#include "assets/mesh.h"
#include "assets/material.h"


/*
class RenderBatch{
public:
	static void AddObject(Mesh& mesh, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform, const std::string& shaderName);
	static void Setup();
	static void Clear();
	static void Draw(const std::shared_ptr<Shader>& shader);
public:
	static std::vector<Vertex> vertices;
	static std::vector<unsigned int> indices;

	static std::vector<glm::mat4> matrixList;
	static std::vector<glm::vec3> colorList;

private:
	static unsigned int VAO, VBO, EBO;
	static int index;
}
*/

class BatchRenderer{
public:
	static void AddObject(Mesh& mesh, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform, const std::string& shaderName);
	static void Setup();
	static void Clear();
	static void Draw(const std::shared_ptr<Shader>& shader);
public:
	static std::vector<Vertex> vertices;
	static std::vector<unsigned int> indices;

	static std::vector<glm::mat4> matrixList;

	static std::unordered_map<std::string, std::any> materialMap;


private:
	static void AddPropertyVector(std::shared_ptr<Material>& material, int& i);
	static void AddProperty(std::shared_ptr<Material>& material, int& i); 
private:
	static unsigned int VAO, VBO, EBO;
	static int index;



};
