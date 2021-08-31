#include "mypch.h"
#include "core/batchRenderer.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "core/engineInfo.h"
#include "core/input.h"
#include "core/renderer.h"

std::unordered_map<std::string, std::vector<Batch>> BatchRenderer::batchMap;
std::unordered_map<std::string, unsigned long> BatchRenderer::batchIndexes;

std::unordered_map<std::string, std::vector<LightBatch>>  LightsBatchRenderer::pointLightBatchMap;
std::unordered_map<std::string, std::vector<LightBatch>>  LightsBatchRenderer::directionalLightBatchMap;
std::unordered_map<std::string, std::vector<LightBatch>>  LightsBatchRenderer::spotLightBatchMap;
std::unordered_map<std::string, unsigned long> LightsBatchRenderer::pointLightBatchIndexes;
std::unordered_map<std::string, unsigned long> LightsBatchRenderer::directionalLightBatchIndexes;
std::unordered_map<std::string, unsigned long> LightsBatchRenderer::spotLightBatchIndexes;

std::vector<BasicVertex> LightsBatchRenderer::quadVertices;
std::vector<unsigned int> LightsBatchRenderer::quadIndices;

std::vector<BasicVertex> LightsBatchRenderer::sphereVertices;
std::vector<unsigned int> LightsBatchRenderer::sphereIndices;

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
	matrixList.clear();

	index = 0;
	textureIndex = 0;
}
void Batch::Destroy() {
	Clear();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Batch::AddProperty(const Material* const material, unsigned long& i) {
	const auto& materialType = material->materialProperties[i].second.index();
	switch(materialType) {
	case 0:
		std::get<std::vector<float>>(materialMap[material->materialProperties[i].first]).push_back(std::get<float>(material->materialProperties[i].second));
		break;
	case 1:
		std::get<std::vector<int>>(materialMap[material->materialProperties[i].first]).push_back(std::get<int>(material->materialProperties[i].second));
		break;
	case 2:
		std::get<std::vector<glm::vec3>>(materialMap[material->materialProperties[i].first]).push_back(std::get<glm::vec3>(material->materialProperties[i].second));
		break;
	case 3:
		std::get<std::vector<glm::vec4>>(materialMap[material->materialProperties[i].first]).push_back(std::get<glm::vec4>(material->materialProperties[i].second));
		break;
	case 4:
		std::string textureName = std::get<TextureInfo>(material->materialProperties[i].second).name;

		if(textureIndexMap.find(textureName) == textureIndexMap.end()) {
			textureIndexMap[textureName] = textureIndex;
			textureIndex++;
		}
		std::get<std::vector<int>>(materialMap[material->materialProperties[i].first]).push_back(textureIndexMap[textureName]);
		break;
	}
}
void Batch::AddPropertyVector(const Material* const material, unsigned long& i) {
	const auto& materialType = material->materialProperties[i].second.index();

	switch(materialType) {
	case 0:
		materialMap[material->materialProperties[i].first] = std::vector<float>();
		break;
	case 1:
		materialMap[material->materialProperties[i].first] = std::vector<int>();
		break;
	case 2:
		materialMap[material->materialProperties[i].first] = std::vector<glm::vec3>();
		break;
	case 3:
		materialMap[material->materialProperties[i].first] = std::vector<glm::vec4>();
		break;
	case 4:
		materialMap[material->materialProperties[i].first] = std::vector<int>();
		break;
	}
}
void Batch::AddProperties(const Material* material, Transform* transform){
	for(unsigned long i = 0; i < material->materialProperties.size(); i++) {
		if(materialMap.count(material->materialProperties[i].first) ==  0) { //if the vector doesnt exits
			AddPropertyVector(material, i);
		}
		AddProperty(material, i);
	}
	matrixList.push_back(transform->GetTransform());
}
void Batch::AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, const Material* material, Transform* transform)  {
	AddProperties(material, transform);

	const std::size_t numNewVertices = otherVertices.size();
	const std::size_t numNewIndices = otherIndices.size();

	const std::size_t numVertices = vertices.size();
	const std::size_t numIndices = indices.size();

	//vertices.reserve(numVertices + numNewVertices);
	//indices.reserve(numIndices + numNewIndices);

	vertices.insert(vertices.end(), otherVertices.begin(), otherVertices.end());
	indices.insert(indices.end(), otherIndices.begin(), otherIndices.end());

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
void Batch::SetProperties(Shader* const shader) {
	for(auto& property : materialMap) {
		const auto& materialType = property.second.index();
		switch(materialType) {
		case 0: {
			const auto& propertyVector = std::get<std::vector<float>>(property.second);
			glUniform1fv(shader->GetUniformLocation(property.first.c_str()), propertyVector.size(), &propertyVector[0]);
			break;
		}
		case 1: {
			const auto& propertyVector = std::get<std::vector<int>>(property.second);
			glUniform1iv(shader->GetUniformLocation(property.first.c_str()), propertyVector.size(), &propertyVector[0]);
			break;
		}
		case 2: {
			const auto& propertyVector = std::get<std::vector<glm::vec3>>(property.second);
			glUniform3fv(shader->GetUniformLocation(property.first.c_str()), propertyVector.size(), &propertyVector[0][0]);
			break;
		}
		case 3: {
			const auto& propertyVector = std::get<std::vector<glm::vec4>>(property.second);
			glUniform4fv(shader->GetUniformLocation(property.first.c_str()), propertyVector.size(), &propertyVector[0][0]);
			break;
		}
		case 4: {
			const auto& propertyVector = std::get<std::vector<int>>(property.second);
			glUniform1iv(shader->GetUniformLocation(property.first.c_str()), propertyVector.size(), &propertyVector[0]);
			break;
		}
		}
	}
}

void Batch::Draw(Shader* shader) {
	if (vertices.size() == 0) {
		return;
	}
	assert(vertices.size() != 0);
	assert(indices.size() != 0);
	assert(index == matrixList.size());

	SetProperties(shader);

	for(auto& a : textureIndexMap) {
		glActiveTexture(GL_TEXTURE0 + a.second); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, TextureManager::textureMap[a.first]);
	}
	glUniformMatrix4fv(glGetUniformLocation(shader->GetID(), "matModel"), matrixList.size(), GL_FALSE, &matrixList[0][0][0]);

	glBindVertexArray(VAO);

	int size;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	if(vertices.size() * sizeof(Vertex) <= size) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STREAM_DRAW);
		//Log("Vertices current size: " << size << " desired size: " << vertices.size() * sizeof(Vertex));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	if(indices.size() * sizeof(unsigned int) <= size) {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), &indices[0]);
	}
	else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STREAM_DRAW);
		//Log("Indices current size: " << size << " desired size: " << vertices.size() * sizeof(Vertex));
	}

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (const void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Batch::DrawThisGeometry(Shader* const shader, const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices){
	SetProperties(shader);

	for(auto& a : textureIndexMap) {
		glActiveTexture(GL_TEXTURE0 + a.second); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, TextureManager::textureMap[a.first]);
	}
	glUniformMatrix4fv(glGetUniformLocation(shader->GetID(), "matModel"), matrixList.size(), GL_FALSE, &matrixList[0][0][0]);

	glBindVertexArray(VAO);

	int size;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	if(otherVertices.size() * sizeof(Vertex) <= size) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, otherVertices.size() * sizeof(Vertex), &otherVertices[0]);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, otherVertices.size() * sizeof(Vertex), &otherVertices[0], GL_STREAM_DRAW);
		//Log("Vertices current size: " << size << " desired size: " << otherVertices.size() * sizeof(Vertex));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	if(otherIndices.size() * sizeof(unsigned int) <= size) {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, otherIndices.size() * sizeof(unsigned int), &otherIndices[0]);
	}
	else {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, otherIndices.size() * sizeof(unsigned int), &otherIndices[0], GL_STREAM_DRAW);
		//Log("Indices current size: " << size << " desired size: " << otherVertices.size() * sizeof(Vertex));
	}

	glDrawElements(GL_TRIANGLES, (GLsizei)otherIndices.size(), GL_UNSIGNED_INT, (const void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



LightBatch::LightBatch(LightBatch&& other) noexcept {
	index = std::move(other.index);

	vertices = std::move(other.vertices);
	indices = std::move(other.indices);

	matrixList = std::move(other.matrixList);

	pointLightList = std::move(other.pointLightList);
	directionalLightList = std::move(other.directionalLightList);

	VAO = std::move(other.VAO);
	VBO = std::move(other.VBO);
	EBO = std::move(other.EBO);
	UBO = std::move(other.UBO);
}
void LightBatch::Setup(LightBatchType otherType) {
	type = otherType;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &UBO);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, UBO);
	switch(type) {
	case LightBatchType::Point:
		glBufferData(GL_SHADER_STORAGE_BUFFER, LIGHT_BATCH_SIZE * sizeof(PointLight), nullptr, GL_STREAM_DRAW);
		break;
	case LightBatchType::Directional:
		glBufferData(GL_SHADER_STORAGE_BUFFER, LIGHT_BATCH_SIZE * sizeof(DirectionalLight), nullptr, GL_STREAM_DRAW);
		break;
	case LightBatchType::Spot:
		glBufferData(GL_SHADER_STORAGE_BUFFER, LIGHT_BATCH_SIZE * sizeof(SpotLight), nullptr, GL_STREAM_DRAW);
		break;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, UBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (void*)offsetof(BasicVertex, ObjectIndex));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void LightBatch::AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, PointLight& light, Transform* transform) {

	const std::size_t numVertices = vertices.size();
	const std::size_t numIndices = indices.size();

	const std::size_t numNewVertices = otherVertices.size();
	const std::size_t numNewIndices = otherIndices.size();
	//vertices.reserve(numVertices + numNewVertices);
	//indices.reserve(numIndices + numNewIndices);

	vertices.insert(vertices.end(), otherVertices.begin(), otherVertices.end());
	indices.insert(indices.end(), otherIndices.begin(), otherIndices.end());

	matrixList.push_back(transform->GetTransform());
	pointLightList.push_back(light);

	for (std::size_t i = numVertices; i < numVertices + numNewVertices; i++) {
		vertices[i].ObjectIndex = index;
	}
	for (std::size_t i = numIndices; i < numIndices + numNewIndices; i++) {
		indices[i] += numVertices;
	}

	index++;
	assert(index <= LIGHT_BATCH_SIZE);
	assert(matrixList.size() <= LIGHT_BATCH_SIZE);
}
void LightBatch::AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, DirectionalLight& light) {
	const std::size_t numNewVertices = otherVertices.size();
	const std::size_t numNewIndices = otherIndices.size();

	const std::size_t numVertices = vertices.size();
	const std::size_t numIndices = indices.size();

	//vertices.reserve(numVertices + numNewVertices);
	//indices.reserve(numIndices + numNewIndices);

	vertices.insert(vertices.end(), otherVertices.begin(), otherVertices.end());
	indices.insert(indices.end(), otherIndices.begin(), otherIndices.end());

	directionalLightList.push_back(light);

	for (std::size_t i = numVertices; i < numVertices + numNewVertices; i++) {
		vertices[i].ObjectIndex = index;
	}
	for (std::size_t i = numIndices; i < numIndices + numNewIndices; i++) {
		indices[i] += numVertices;
	}

	index++;
	assert(index <= LIGHT_BATCH_SIZE);
	assert(matrixList.size() <= LIGHT_BATCH_SIZE);
}
void LightBatch::Draw(Shader* shader) {
	if (vertices.size() == 0) {
		return;
	}
	assert(vertices.size() != 0);
	assert(indices.size() != 0);

	glUniformMatrix4fv(glGetUniformLocation(shader->GetID(), "matModel"), matrixList.size(), GL_FALSE, &matrixList[0][0][0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, UBO);
	/*
	switch(type) {
	case LightBatchType::Point:
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, index * sizeof(PointLight), &pointLightList[0]);
		break;
	case LightBatchType::Directional:
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, index * sizeof(DirectionalLight), &directionalLightList[0]);
		break;
	case LightBatchType::Spot:
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, index * sizeof(SpotLight), &pointLightList);
		break;
	}
	*/
	switch(type) {
	case LightBatchType::Point:
		glBufferData(GL_SHADER_STORAGE_BUFFER,index * sizeof(PointLight), &pointLightList[0], GL_STREAM_DRAW);
		break;
	case LightBatchType::Directional:
		glBufferData(GL_SHADER_STORAGE_BUFFER,index * sizeof(DirectionalLight), &directionalLightList[0], GL_STREAM_DRAW);
		break;
	case LightBatchType::Spot:
		glBufferData(GL_SHADER_STORAGE_BUFFER,index * sizeof(SpotLight), &pointLightList, GL_STREAM_DRAW);
		break;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, UBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(BasicVertex), &vertices[0], GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STREAM_DRAW);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (const void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void LightBatch::Clear() {
	vertices.clear();
	indices.clear();

	pointLightList.clear();
	directionalLightList.clear();
	//spotLightList.clear();
	matrixList.clear();

	index = 0;
}
void LightBatch::Destroy() {
	Clear();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &UBO);
	glDeleteVertexArrays(1, &VAO);
}


void BatchRenderer::Clear() {
	for(auto& batchPair : batchMap) {
		for(unsigned long i = 0; i < batchIndexes[batchPair.first] + 1; i++) {
			batchPair.second[i].Clear();
		}
		for(unsigned long i = batchIndexes[batchPair.first] + 1; i < batchPair.second.size(); i++) {
			batchPair.second[i].Destroy();
		}
	}
	for(auto& batchPair : batchIndexes) {
		batchPair.second = 0;
	}
}
void BatchRenderer::AddObject(const std::vector<Vertex>& otherVertices, const std::vector<unsigned int>& otherIndices, const Material* material, Transform* transform, const std::string& shaderName)  {
	std::vector<Batch>& batchList = batchMap[shaderName];
	unsigned long& batchIndex = batchIndexes[shaderName];

	if(batchList.empty()) {
		batchList.emplace_back().Setup();
		batchIndex = 0;
	}
	else if(batchList[batchIndex].index == BATCH_SIZE || batchList[batchIndex].textureIndex == 16) { //todo change the texture index max value to a define or smth
		if(batchIndex < batchList.size() - 1) {
			batchIndex++;
		}
		else {
			batchList.emplace_back().Setup();
			batchIndex++;
		}
	}
	batchList[batchIndex].AddObject(otherVertices, otherIndices, material, transform);
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
void BatchRenderer::Draw() {
	for(auto& batchPair : batchMap) {
		auto shader = Shader::shaderMap[batchPair.first].get();
		shader->use();
		for(auto& batch : batchPair.second) {
			batch.Draw(shader);
		}
	}
}
void BatchRenderer::Destroy(){
	for(auto& batchPair : batchMap) {
		for(auto& batch : batchPair.second){
			batch.Destroy();
		}
	}
	BatchRenderer::batchMap.clear();
	BatchRenderer::batchIndexes.clear();
}

void LightsBatchRenderer::Init() {
	quadVertices = {
		BasicVertex(-1.0f, -1.0f, 0.0f, 0),
		BasicVertex( 1.0f, -1.0f, 0.0f, 0),
		BasicVertex( 1.0f,  1.0f, 0.0f, 0),
		BasicVertex(-1.0f,  1.0f, 0.0f, 0)
	};
	quadIndices = {
		0, 1, 2,
		0, 2, 3
	};
	std::vector<glm::vec3> tempVertices;
	//ModelImporter::LoadModelBasic("res/fbx/sphere.fbx", tempVertices, sphereIndices);
	ModelImporter::LoadModelBasic("res/fbx/box.fbx", tempVertices, sphereIndices);
	sphereVertices = std::vector<BasicVertex>(tempVertices.begin(), tempVertices.end());
}
void LightsBatchRenderer::AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, PointLight& light, Transform* transform, const std::string& shaderName) {
	std::vector<LightBatch>& batchList = pointLightBatchMap[shaderName];
	unsigned long& batchIndex = pointLightBatchIndexes[shaderName];

	if(batchList.empty()) {
		batchList.emplace_back().Setup(LightBatchType::Point);
		batchIndex = 0;
	}
	else if(batchList[batchIndex].index == LIGHT_BATCH_SIZE) { //todo change the texture index max value to a define or smth
		if(batchIndex < batchList.size() - 1) {
			batchIndex++;
		}
		else {
			batchList.emplace_back().Setup(LightBatchType::Point);
			batchIndex++;
		}
	}
	batchList[batchIndex].AddObject(otherVertices, otherIndices, light, transform);
}
void LightsBatchRenderer::AddObject(const std::vector<BasicVertex>& otherVertices, const std::vector<unsigned int>& otherIndices, DirectionalLight& light, Transform* transform, const std::string& shaderName) {
	std::vector<LightBatch>& batchList = directionalLightBatchMap[shaderName];
	unsigned long& batchIndex = directionalLightBatchIndexes[shaderName];

	if(batchList.empty()) {
		batchList.emplace_back().Setup(LightBatchType::Directional);
		batchIndex = 0;
	}
	else if(batchList[batchIndex].index == LIGHT_BATCH_SIZE) { //todo change the texture index max value to a define or smth
		if(batchIndex < batchList.size() - 1) {
			batchIndex++;
		}
		else {
			batchList.emplace_back().Setup(LightBatchType::Directional);
			batchIndex++;
		}
	}
	batchList[batchList.size() - 1].AddObject(otherVertices, otherIndices, light);
}
void LightsBatchRenderer::Clear() {
	for(auto& batchPair : pointLightBatchMap) {
		for(unsigned long i = 0; i < pointLightBatchIndexes[batchPair.first] + 1; i++) {
			batchPair.second[i].Clear();
		}
		for(unsigned long i = pointLightBatchIndexes[batchPair.first] + 1; i < batchPair.second.size(); i++) {
			batchPair.second[i].Destroy();
		}
	}
	for(auto& batchPair : directionalLightBatchMap) {
		for(unsigned long i = 0; i < directionalLightBatchIndexes[batchPair.first] + 1; i++) {
			batchPair.second[i].Clear();
		}
		for(unsigned long i = directionalLightBatchIndexes[batchPair.first] + 1; i < batchPair.second.size(); i++) {
			batchPair.second[i].Destroy();
		}
	}
	for(auto& batchPair : spotLightBatchMap) {
		for(unsigned long i = 0; i < spotLightBatchIndexes[batchPair.first] + 1; i++) {
			batchPair.second[i].Clear();
		}
		for(unsigned long i = spotLightBatchIndexes[batchPair.first] + 1; i < batchPair.second.size(); i++) {
			batchPair.second[i].Destroy();
		}
	}
	for(auto& batchPair : pointLightBatchIndexes) {
		batchPair.second = 0;
	}
	for(auto& batchPair : directionalLightBatchIndexes) {
		batchPair.second = 0;
	}
	for(auto& batchPair : spotLightBatchIndexes) {
		batchPair.second = 0;
	}
}
void LightsBatchRenderer::Draw() {
	if(pointLightBatchMap.size() + directionalLightBatchMap.size() == 0) {
		return;
	}

	glEnable(GL_STENCIL_TEST);

	auto nullShader = Shader::shaderMap["res/shaders/null"].get();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	nullShader->use();
	for(auto& batchPair : pointLightBatchMap) {
		for(auto& batch : batchPair.second) {
			batch.Draw(nullShader);
		}
	}
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	for(auto& batchPair : pointLightBatchMap) {
		auto shader = Shader::shaderMap[batchPair.first].get();
		shader->use();
		shader->setVec3("eyePos", Renderer::viewPos);
		shader->setVec2("gScreenSize", {EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT});
		shader->setFloat("specularPower", 1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Renderer::multisampledFrameBuffer.m_color);
		shader->setInt("gColor", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Renderer::multisampledFrameBuffer.m_position);
		shader->setInt("gPosition", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Renderer::multisampledFrameBuffer.m_normal);
		shader->setInt("gNormal", 2);

		for(auto& batch : batchPair.second) {
			batch.Draw(shader);
		}
	}
	glDisable(GL_STENCIL_TEST);
	for(auto& batchPair : directionalLightBatchMap) {
		auto shader = Shader::shaderMap[batchPair.first].get();
		shader->use();
		for(auto& batch : batchPair.second) {
			batch.Draw(shader);
		}
	}
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
/*
void LightsBatchRenderer::Draw() {
	if(pointLightBatchMap.size() + directionalLightBatchMap.size() == 0){
		return;
	}
	for(auto& batchPair : pointLightBatchMap) {
		auto& shader = Shader::shaderMap[batchPair.first];
		shader->use();
		shader->setVec3("eyePos", Renderer::viewPos);
		shader->setVec2("gScreenSize", {EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT});
		shader->setFloat("specularPower", 1.0f);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, Renderer::multisampledFrameBuffer.m_color); shader->setInt("gColor", 0);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, Renderer::multisampledFrameBuffer.m_position); shader->setInt("gPosition", 1);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, Renderer::multisampledFrameBuffer.m_normal); shader->setInt("gNormal", 2);

		for(auto& batch : batchPair.second) {
			batch.Draw(shader);
		}
	}
	for(auto& batchPair : directionalLightBatchMap) {
		auto& shader = Shader::shaderMap[batchPair.first];
		shader->use();
		for(auto& batch : batchPair.second) {
			batch.Draw(shader);
		}
	}
}
*/
void LightsBatchRenderer::Destroy(){
	for(auto& batchPair : pointLightBatchMap) {
		for(auto& batch : batchPair.second){
			batch.Destroy();
		}
	}
	for(auto& batchPair : directionalLightBatchMap) {
		for(auto& batch : batchPair.second){
			batch.Destroy();
		}
	}
	for(auto& batchPair : spotLightBatchMap) {
		for(auto& batch : batchPair.second){
			batch.Destroy();
		}
	}
	LightsBatchRenderer::pointLightBatchMap.clear();
	LightsBatchRenderer::directionalLightBatchMap.clear();
	LightsBatchRenderer::spotLightBatchMap.clear();
	LightsBatchRenderer::pointLightBatchIndexes.clear();
	LightsBatchRenderer::directionalLightBatchIndexes.clear();
	LightsBatchRenderer::spotLightBatchIndexes.clear();
}
