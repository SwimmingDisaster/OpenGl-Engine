#pragma once
#include "assets/mesh.h"
#include "assets/material.h"

#define BATCH_SIZE 150

class Batch{
public:
	~Batch();
public:
	void AddObject(Mesh& mesh, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform);
	void Setup();
	void Clear();
	void Draw(const std::shared_ptr<Shader>& shader);
public:
	int index = 0;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<glm::mat4> matrixList;

	std::unordered_map<std::string, std::any> materialMap;
	std::unordered_map<std::string, int> textureIndexMap;

	int textureIndex = 0;

private:
	void AddPropertyVector(std::shared_ptr<Material>& material, int& i);
	void AddProperty(std::shared_ptr<Material>& material, int& i); 
private:
	unsigned int VAO, VBO, EBO;
};

class BatchRenderer{
public:
	static std::unordered_map<std::string, std::vector<Batch>> batchMap;
	static void AddObject(Mesh& mesh, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform, const std::string& shaderName);
	static void Clear();
	static void Draw();
};
