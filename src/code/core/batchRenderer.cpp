#include "mypch.h"
#include "core/batchRenderer.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "core/input.h"

std::unordered_map<std::string, std::vector<Batch>> BatchRenderer::batchMap;
std::unordered_map<std::string, unsigned long> BatchRenderer::batchIndexes;


void Batch::Setup() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, ObjectIndex));
	glEnableVertexAttribArray(5);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Batch::Clear() {
	vertices.clear();
	indices.clear();
	materialMap.clear();

	index = 0;
}

void Batch::AddProperty(std::shared_ptr<Material>& material, unsigned long& i) {
	const auto& materialType = material->materialProperties[i].second.type();
	if(materialType == typeid(float)) {
		std::any_cast<std::vector<float>>(&materialMap[material->materialProperties[i].first])->push_back(*std::any_cast<float>(&material->materialProperties[i].second));
	}
	else if(materialType == typeid(int)) {
		std::any_cast<std::vector<int>>(&materialMap[material->materialProperties[i].first])->push_back(*std::any_cast<int>(&material->materialProperties[i].second));
	}
	else if(materialType == typeid(glm::vec3)) {
		std::any_cast<std::vector<glm::vec3>>(&materialMap[material->materialProperties[i].first])->push_back(*std::any_cast<glm::vec3>(&material->materialProperties[i].second));
	}
	else if(materialType == typeid(glm::vec4)) {
		std::any_cast<std::vector<glm::vec4>>(&materialMap[material->materialProperties[i].first])->push_back(*std::any_cast<glm::vec4>(&material->materialProperties[i].second));
	}
	else if(materialType == typeid(TextureInfo)) {

		std::string textureName = std::any_cast<TextureInfo>(material->materialProperties[i].second).name;

		if(textureIndexMap.find(textureName) == textureIndexMap.end()) {
			textureIndexMap[textureName] = textureIndex;
			textureIndex++;
		}
		std::any_cast<std::vector<int>>(&materialMap[material->materialProperties[i].first])->push_back(textureIndexMap[textureName]);
	}
}

void Batch::AddPropertyVector(std::shared_ptr<Material>& material, unsigned long& i) {
	const auto& materialType = material->materialProperties[i].second.type();

	if(materialType == typeid(float)) {
		materialMap[material->materialProperties[i].first] = std::vector<float>();
	}
	else if(materialType == typeid(int)) {
		materialMap[material->materialProperties[i].first] = std::vector<int>();
	}
	else if(materialType == typeid(glm::vec3)) {
		materialMap[material->materialProperties[i].first] = std::vector<glm::vec3>();
	}
	else if(materialType == typeid(glm::vec4)) {
		materialMap[material->materialProperties[i].first] = std::vector<glm::vec4>();
	}
	else if(materialType == typeid(TextureInfo)) {
		materialMap[material->materialProperties[i].first] = std::vector<int>();
	}
}

void Batch::AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform)  {
	for(unsigned long i = 0; i < material->materialProperties.size(); i++) {
		if(materialMap.count(material->materialProperties[i].first) ==  0) { //if the vector doesnt exits
			AddPropertyVector(material, i);
		}
		AddProperty(material, i);
	}

	const int numNewVertices = otherVertices.size();
	const int numNewIndices = otherIndices.size();

	const int numVertices = vertices.size();
	const int numIndices = indices.size();

	//vertices.reserve(numVertices + numNewVertices);
	//indices.reserve(numIndices + numNewIndices);

	vertices.insert(vertices.end(), otherVertices.begin(), otherVertices.end());
	indices.insert(indices.end(), otherIndices.begin(), otherIndices.end());

	matrixList.push_back(transform->GetTransform());


	for (std::size_t i = numVertices; i < numVertices + numNewVertices; i++) {
		vertices[i].ObjectIndex = index;
	}

	for (std::size_t i = numIndices; i < numIndices + numNewIndices; i++) {
		indices[i] += numVertices;
	}

	index++;
	assert(index <= BATCH_SIZE);
	assert(matrixList.size() <= BATCH_SIZE);
}

void Batch::Draw(const std::shared_ptr<Shader>& shader) {
	if (vertices.size() == 0) {
		return;
	}
	assert(vertices.size() != 0);
	assert(indices.size() != 0);

	for(auto& property : materialMap) {
		if(property.second.type() == typeid(std::vector<float>)) {
			auto* propertyVector = std::any_cast<std::vector<float>>(&property.second);
			glUniform1fv(glGetUniformLocation(shader->GetID(), property.first.c_str()), (*propertyVector).size(), &((*propertyVector)[0]));
		}
		else if(property.second.type() == typeid(std::vector<int>)) {
			auto* propertyVector = std::any_cast<std::vector<int>>(&property.second);
			glUniform1iv(glGetUniformLocation(shader->GetID(), property.first.c_str()), (*propertyVector).size(), &((*propertyVector)[0]));
		}
		else if(property.second.type() == typeid(std::vector<glm::vec3>)) {
			auto* propertyVector = std::any_cast<std::vector<glm::vec3>>(&property.second);
			glUniform3fv(glGetUniformLocation(shader->GetID(), property.first.c_str()), (*propertyVector).size(), &((*propertyVector)[0][0]));
		}
		else if(property.second.type() == typeid(std::vector<glm::vec4>)) {
			auto* propertyVector = std::any_cast<std::vector<glm::vec4>>(&property.second);
			glUniform4fv(glGetUniformLocation(shader->GetID(), property.first.c_str()), (*propertyVector).size(), &((*propertyVector)[0][0]));
		}
		else if(property.second.type() == typeid(std::vector<int>)) {
			auto* propertyVector = std::any_cast<std::vector<int>>(&property.second);
			glUniform1iv(glGetUniformLocation(shader->GetID(), property.first.c_str()), (*propertyVector).size(), &((*propertyVector)[0]));
		}
	}

	for(auto& a : textureIndexMap) {
		glActiveTexture(GL_TEXTURE0 + a.second); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, TextureManager::textureMap[a.first]);
	}
	glUniformMatrix4fv(glGetUniformLocation(shader->GetID(), "matModel"), matrixList.size(), GL_FALSE, &matrixList[0][0][0]);

	glBindVertexArray(VAO);
	glFinish();


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (const void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glFinish();
}
Batch::Batch(Batch&& other) noexcept {
	index = std::move(other.index);
	textureIndex = std::move(other.textureIndex);

	vertices = std::move(other.vertices);
	indices = std::move(other.indices);

	matrixList = std::move(other.matrixList);

	materialMap = std::move(other.materialMap);
	textureIndexMap = std::move(other.textureIndexMap);

	VAO = std::move(other.VAO);
	VBO = std::move(other.VBO);
	EBO = std::move(other.EBO);
}

void Batch::Destroy() {
	Clear();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

//Batch::~Batch() {}

void BatchRenderer::AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform, const std::string& shaderName)  {
	std::vector<Batch>& batchList = batchMap[shaderName];
	unsigned long& batchIndex = batchIndexes[shaderName];

	if(batchList.empty()) {
		batchList.emplace_back().Setup();
		//batchList.emplace_back();
		//batchList[batchList.size() - 1].Setup();
		batchIndex = 0;
	}
	else if(batchList[batchIndex].index == BATCH_SIZE || batchList[batchIndex].textureIndex == 16){//todo change the texture index max value to a define or smth
		if(batchIndex < batchList.size() - 1){
			batchIndex++;
		}
		else{
			//batchList.emplace_back();
			//batchList[batchList.size() - 1].Setup();
			batchList.emplace_back().Setup();
			batchIndex++;
		}
	}
	batchList[batchList.size() - 1].AddObject(otherVertices, otherIndices, material, transform);
	/*
	batchList[batchList.size() - 1].AddObject(otherVertices, otherIndices, material, transform);
	std::vector<Batch>& batchList= batchMap[shaderName];
	if(batchList.empty()) {
		batchList.emplace_back();
		batchList[batchList.size() - 1].Setup();
	}
	else if(batchList[batchList.size() - 1].index == BATCH_SIZE) {
		batchList.emplace_back();
		batchList[batchList.size() - 1].Setup();
	}
	batchList[batchList.size() - 1].AddObject(otherVertices, otherIndices, material, transform);
	*/
}

void BatchRenderer::Clear() {
	for(auto& batchPair : batchMap) {
		for(unsigned long i = 0; i < batchIndexes[batchPair.first]; i++){
			batchPair.second[i].Clear();
		}
		for(unsigned long i = batchIndexes[batchPair.first]; i < batchPair.second.size(); i++) {
			batchPair.second[i].Destroy();
		}
	}
	batchMap.clear();
}

void BatchRenderer::Draw() {
	for(auto& batchPair : batchMap) {
		auto& shader = Shader::shaderMap[batchPair.first];
		shader->use();
		for(auto& batch : batchPair.second) {
			batch.Draw(shader);
		}
	}
}