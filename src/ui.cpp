#include "mypch.h"
#include "ui.h"
#include "shader.h"

#include "game.h"
#include "texture.h"


Font::Font() {
}

void Font::Init(std::string fileName, std::string textureName) {
	std::ifstream infile(fileName);
	std::string line;
	int num = 0;
	for (int i = 0; i < 3; i++) {
		std::getline(infile, line);
	}
	std::getline(infile, line);
	std::vector<std::string> strs;


	boost::split(strs, line, boost::is_any_of("\t= "), boost::token_compress_on);
	num = stoi(strs[2]);
	for (int i = 0; i < num; i++)
	{
		std::getline(infile, line);
		//std::cout << line << std::endl;
		boost::split(strs, line, boost::is_any_of("\t= "), boost::token_compress_on);
		//std::cout << strs.size() << std::endl;
		int id = stoi(strs[2]);
		float x = (float)stoi(strs[4]) / 512.0f;
		float y = (float)stoi(strs[6]) / 512.0f;
		float sizeX = (float)stoi(strs[8]) / 512.0f;
		float sizeY = (float)stoi(strs[10]) / 512.0f;
		float xMax = x + sizeX;
		float yMax = y + sizeY;
		float xOffset = (float)stoi(strs[12]) / 512.0f;
		float yOffset = (float)stoi(strs[14]) / 512.0f;
		float xAdvance = (float)stoi(strs[16]) / 512.0f;
		Char_map[stoi(strs[2])] = Character(id, x, y, xMax, yMax, xOffset, yOffset, sizeX, sizeY, xAdvance);
	}

	atlasTexture = loadTexture(textureName.c_str());
	spaceWidth = Char_map[32].xAdvance;
}



std::vector<Word> UI_text::WordsFromText(std::string f_text) {
	std::vector<std::string> strs;

	boost::split(strs, f_text, boost::is_any_of("\t "), boost::token_compress_on);
	unsigned int numWords = strs.size();
	std::vector<Word> words(numWords);

	for (int i = 0; i < numWords; i++) {
		words[i].characters.reserve(strs[i].length());
		for (int j = 0; j < strs[i].length(); j++) {
			words[i].characters.emplace_back(font->Char_map[strs[i][j]]);
		}
	}
	return words;
}

std::vector<Line> UI_text::LinesFromWords(std::vector<Word> words) {
	std::vector<Line> lines(1);
	float lineSize = 0;
	unsigned int index = 0;
	for (int i = 0; i < words.size(); i++) {
		if (lineSize + getSize(words[i]) * fontSize < lineMaxWidth * Game::SCREEN_WIDTH) {
			lines[index].words.push_back(words[i]);
		}
		else {
			index++;
			lineSize = 0.0f;
			Line lin;
			lin.words.push_back(words[i]);
			lines.push_back(lin);

		}
		lineSize += getSize(words[i]) * fontSize + font->spaceWidth * fontSize;
	}
	return lines;
}

float UI_text::getSize(Word word) {
	float size = 0.0f;
	for (Character& c : word.characters) {
		size += c.xAdvance;
	}
	return size;
}

float UI_text::getSize(Line line) {
	float size = 0.0f;
	for (Word& w : line.words) {
		size += getSize(w);
	}
	size += (line.words.size() - 1) * font->spaceWidth;
	return size;
}

UI_text::UI_text() {}

void UI_text::Init(std::string c_text, Font* c_font) {
	text = c_text;
	font = c_font;
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &vertexVBO);
	glGenBuffers(1, &textureVBO);
	RecalculateText(c_text);
}

void UI_text::addVerticesForCharacter(Character& chara, float curX, float curY) {
	float x = curX + (chara.xOffset * fontSize);
	float y = curY + (chara.yOffset * fontSize);
	float maxX = x + (chara.sizeX * fontSize);
	float maxY = y + (chara.sizeY * fontSize);
	float properX = (2 * x) - 1;
	float properY = (-2 * y) + 1;
	float properMaxX = (2 * maxX) - 1;
	float properMaxY = (-2 * maxY) + 1;
	addVertexPos(properX, properY, properMaxX, properMaxY);
}

void UI_text::addVertexPos(float x, float y, float maxX, float maxY) {
	vertexCoord.push_back(x);
	vertexCoord.push_back(y);
	vertexCoord.push_back(x);
	vertexCoord.push_back(maxY);
	vertexCoord.push_back(maxX);
	vertexCoord.push_back(maxY);
	vertexCoord.push_back(maxX);
	vertexCoord.push_back(maxY);
	vertexCoord.push_back(maxX);
	vertexCoord.push_back(y);
	vertexCoord.push_back(x);
	vertexCoord.push_back(y);
}

void UI_text::addTexCoords(float x, float y, float maxX, float maxY) {
	textureCoord.push_back(x);
	textureCoord.push_back(y);
	textureCoord.push_back(x);
	textureCoord.push_back(maxY);
	textureCoord.push_back(maxX);
	textureCoord.push_back(maxY);
	textureCoord.push_back(maxX);
	textureCoord.push_back(maxY);
	textureCoord.push_back(maxX);
	textureCoord.push_back(y);
	textureCoord.push_back(x);
	textureCoord.push_back(y);
}



void UI_text::RecalculateText(std::string f_text) {

	vertexCoord.clear();
	textureCoord.clear();
	std::vector<Word> words = WordsFromText(f_text);
	std::vector<Line> lines = LinesFromWords(words);

	float lineMaxWidthMiddle = (lineMaxWidth / 2) * Game::SCREEN_WIDTH;

	float cursorX = -lineMaxWidthMiddle, cursorY = 0;
	cursorY -=  lines.size() * lineMaxHeight * fontSize / 2;
	for (Line& l : lines) {
		if (isCentered) {
			cursorX = ((lineMaxWidth * Game::SCREEN_WIDTH - getSize(l) * fontSize) / 2) - lineMaxWidthMiddle;
		}
		for (Word& w : l.words) {
			for (Character& c : w.characters) {
				float why = cursorX;
				addVerticesForCharacter(c, cursorX, cursorY);
				addTexCoords(c.xTextureCoord, c.yTextureCoord, c.xMaxTextureCoord, c.yMaxTextureCoord);
				cursorX += c.xAdvance * fontSize * glyphWidth;
			}
			cursorX += font->spaceWidth * fontSize * glyphWidth;
		}
		cursorX = -lineMaxWidthMiddle;
		cursorY += lineMaxHeight * fontSize;
	}

	if (vertexCoord.size() > 0 ) //IF THE STRIGN IS NOT EMPTY
	{
		glBindVertexArray(textVAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, vertexCoord.size() * sizeof(float), &vertexCoord[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glBufferData(GL_ARRAY_BUFFER, textureCoord.size() * sizeof(float), &textureCoord[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, (void*)0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}
}


void UI_text::Draw(Shader & shader) {
	shader.use();
	shader.setFloat("text", font->atlasTexture);
	shader.setVec3("textColor", color);
	shader.setVec2("position", vPos);

	shader.setFloat("width", width);
	shader.setFloat("edge", edge);




	/*	glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::translate(proj, vPos);
		proj = glm::scale(proj, vScale);
		proj = glm::rotate(proj, glm::radians(vRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		proj = glm::rotate(proj, glm::radians(vRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		proj = glm::rotate(proj, glm::radians(vRot.z), glm::vec3(0.0f, 0.0f, 1.0f));*/

	glm::mat4 proj = glm::ortho(0.0f, (float)Game::SCREEN_WIDTH, 0.0f, (float)Game::SCREEN_HEIGHT);
	float scale = Game::SCREEN_HEIGHT / 1080.0f;
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
	shader.setMat4("projection", /*Game::projectionMatrix * Game::viewMatrix * */ proj * model);




	glm::mat4 rot = glm::translate(glm::mat4(1.0f), glm::vec3(vPos, 0.0f));
	rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0f));
	shader.setMat4("rotation", rot);

	if (vertexCoord.size() > 0 ) //IF THE STRING IS NOT EMPTY
	{
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font->atlasTexture);

		glBindVertexArray(textVAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, vertexCoord.size() / 2);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}
}

void UI_text::ImGuiDraw() {
	std::string uniqueIndex = std::to_string(textVAO);
	if (ImGui::CollapsingHeader(("Text-" + uniqueIndex).c_str())) {
		bool changed = false;
		changed |= ImGui::InputText(("Text box-" + uniqueIndex).c_str(), &text, ImGuiInputTextFlags_CallbackResize);
		ImGui::DragFloat2(("Position-" + uniqueIndex).c_str(), glm::value_ptr(vPos));
		/*changed |= ImGui::DragFloat3(("Position-" + uniqueIndex).c_str(), glm::value_ptr(vPos));
		changed |= ImGui::DragFloat3(("Scale-" + uniqueIndex).c_str(), glm::value_ptr(vScale));
		changed |= ImGui::DragFloat3(("Rotation-" + uniqueIndex).c_str(), glm::value_ptr(vRot));*/
		ImGui::ColorEdit3(("Color-" + uniqueIndex).c_str(), glm::value_ptr(color));
		ImGui::DragFloat(("Rotation-" + uniqueIndex).c_str(),  &angle, 0.15f);
		changed |= ImGui::Checkbox(("Is Centered-" + uniqueIndex).c_str(), &isCentered);
		changed |= ImGui::DragFloat(("Font size-" + uniqueIndex).c_str(),  &fontSize);
		changed |= ImGui::DragFloat(("line max width-" + uniqueIndex).c_str(),  &lineMaxWidth, 0.001f);
		changed |= ImGui::DragFloat(("line max height-" + uniqueIndex).c_str(),  &lineMaxHeight, 0.01f);
		changed |= ImGui::DragFloat(("Glyph Width-" + uniqueIndex).c_str(),  &glyphWidth, 0.001f);
		ImGui::DragFloat(("Width-" + uniqueIndex).c_str(),  &width, 0.001f);
		ImGui::DragFloat(("Edge-" + uniqueIndex).c_str(),  &edge, 0.001f);
		if (changed) {
			RecalculateText(text);
		}
	}
}