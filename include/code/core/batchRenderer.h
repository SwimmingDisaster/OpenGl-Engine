#pragma once
#include "core/modelImporter.h"         // for Vertex

class Material;
class Shader;
class Transform;
struct DirectionalLight;
struct PointLight;

#define BATCH_SIZE 150
#define LIGHT_BATCH_SIZE 50

typedef std::variant<std::vector<float>, std::vector<int>, std::vector<glm::vec3>, std::vector<glm::vec4>, std::vector<TextureInfo>> BatchPropertyType;

enum class LightBatchType{
	Point,
	Directional,
	Spot
};

class Batch{
public:
	Batch() = default;
    Batch(const Batch& other) = delete;
	Batch(Batch&& other) noexcept;
public:
	void AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, const Material* material, Transform* transform);
	void Setup();
	void Clear();
	void Destroy();
	void Draw(Shader* const shader);
	void DrawThisGeometry(Shader* shader, const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices);

	void SetProperties(Shader* const shader);
	void AddProperties(const Material* material, Transform* transform);
public:
	std::size_t index = 0;
	std::size_t textureIndex = 0;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<glm::mat4> matrixList;

	std::unordered_map<std::string, BatchPropertyType> materialMap;
	std::unordered_map<std::string, int> textureIndexMap;


private:
	void AddPropertyVector(const Material* material, unsigned long& i);
	void AddProperty(const Material* material, unsigned long& i); 
private:
	unsigned int VAO, VBO, EBO;
};
class LightBatch{
public:
	LightBatch() = default;
    LightBatch(const LightBatch& other) = delete;
	LightBatch(LightBatch&& other) noexcept;
public:
	void AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, PointLight& light, Transform* transform);
	void AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, DirectionalLight& light);
	//void AddObject(const std::vector<glm::vec3>& otherVertices, const std::vector<unsigned int>& otherIndices, PointLight& light, std::shared_ptr<Transform>& transform);
	void Setup(LightBatchType otherType);
	void Clear();
	void Destroy();
	void Draw(Shader* shader);
public:
	std::size_t index = 0;

	LightBatchType type;

	std::vector<BasicVertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<glm::mat4> matrixList;

	std::vector<PointLight> pointLightList;
	std::vector<DirectionalLight> directionalLightList;

private:
	unsigned int VAO, VBO, EBO, UBO;
};

class BatchRenderer{
public:
	static std::unordered_map<std::string, std::vector<Batch>> batchMap;
	static std::unordered_map<std::string, unsigned long> batchIndexes;

public:
	static void AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, const Material* material, Transform* transform, const std::string& shaderName);
	static void Clear();
	static void Draw();
	static void Destroy();
};
class LightsBatchRenderer{
public:
	static std::unordered_map<std::string, std::vector<LightBatch>> pointLightBatchMap;
	static std::unordered_map<std::string, std::vector<LightBatch>> directionalLightBatchMap;
	static std::unordered_map<std::string, std::vector<LightBatch>> spotLightBatchMap;
	static std::unordered_map<std::string, unsigned long> pointLightBatchIndexes;
	static std::unordered_map<std::string, unsigned long> directionalLightBatchIndexes;
	static std::unordered_map<std::string, unsigned long> spotLightBatchIndexes;

	static std::vector<BasicVertex> quadVertices;
	static std::vector<unsigned int> quadIndices;

	static std::vector<BasicVertex> sphereVertices;
	static std::vector<unsigned int> sphereIndices;
public:
	static void Init();
	static void AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, PointLight& light, Transform* transform, const std::string& shaderName);
	static void AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, DirectionalLight& light, Transform* transform, const std::string& shaderName);
	static void Clear();
	static void Draw();
	static void Destroy();
};
