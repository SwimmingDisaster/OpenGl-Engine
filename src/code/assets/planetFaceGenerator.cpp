#include "assets/planetFaceGenerator.h"

REGISTERIMPL(PlanetFaceGenerator);
GETNAMEIMPL(PlanetFaceGenerator);

void PlanetFaceGenerator::Show()  {
	//	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
	ImGui::SliderInt("Resolution", &resolution, 3, 128);
	if(ImGui::Button("Generate face")){
		GenerateFace();	
	}
}
void PlanetFaceGenerator::Serialize(YAML::Emitter& out) const {
	out << YAML::Key << name;
	out << YAML::BeginMap;

	//	out << YAML::Key << "Position" << YAML::Value << position;

	out << YAML::EndMap;
}
void PlanetFaceGenerator::Deserialize(const YAML::Node& data) {
	//	position = data["Position"].as<glm::vec3>();
}
void PlanetFaceGenerator::Start() {
	model = parentEntity->GetComponent<Model>();
	GenerateFace();
}
void PlanetFaceGenerator::GenerateFace(){
	model->vertices.clear();
	model->indices.clear();
	model->vertices.resize(resolution * resolution);
	model->indices.resize(6 * (resolution - 1) * (resolution - 1));


	int triIndex = 0, i = 0;
	for(int y = 0; y < resolution; ++y) {
		for(int x = 0; x < resolution; ++x) {
			glm::vec2 percent = glm::vec2(x,y) / (float)(resolution - 1);
			//glm::vec3 pointOnUnitCube = localUP
			model->vertices[i].Position = {2.0f * percent.x - 1.0f, 1.0f, 2.0f * percent.y - 1.0f};
			model->vertices[i].Position = glm::normalize(model->vertices[i].Position);
			//Log(model->vertices[i].Position);
			if(x != resolution -1 && y != resolution -1) {
				model->indices[triIndex] = i;
				model->indices[triIndex + 1] = i + resolution;
				model->indices[triIndex + 2] = i + resolution + 1;

				model->indices[triIndex + 3] = i;
				model->indices[triIndex + 4] = i + resolution + 1;
				model->indices[triIndex + 5] = i + 1;
				triIndex += 6;
			}
			i++;
		}
	}
	model->RecalculateNormals();
}
void PlanetFaceGenerator::Update() {

}
void PlanetFaceGenerator::Draw() {

}
void PlanetFaceGenerator::End() {

}

