#pragma once
#include "assets/model.h"
#include "assets/material.h"
#include "core/modelImporter.h"

#define BATCH_SIZE 50

class Batch{
public:
	Batch() = default;
    Batch(const Batch& other) = delete;
	Batch(Batch&& other) noexcept;
public:
	void AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform);
	void Setup();
	void Clear();
	void Destroy();
	void Draw(const std::shared_ptr<Shader>& shader);
public:
	int index = 0;
	int textureIndex = 0;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<glm::mat4> matrixList;

	std::unordered_map<std::string, std::any> materialMap;
	std::unordered_map<std::string, int> textureIndexMap;


private:
	void AddPropertyVector(std::shared_ptr<Material>& material, unsigned long& i);
	void AddProperty(std::shared_ptr<Material>& material, unsigned long& i); 
private:
	unsigned int VAO, VBO, EBO;
};

class BatchRenderer{
public:
	static std::unordered_map<std::string, std::vector<Batch>> batchMap;
	static std::unordered_map<std::string, unsigned long> batchIndexes;

public:
	static void AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform, const std::string& shaderName);
	static void Clear();
	static void Draw();
};
