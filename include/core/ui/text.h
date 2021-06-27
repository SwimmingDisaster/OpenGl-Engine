#pragma once
#include "mypch.h"
#include "core/shader.h"

struct Character {
	int id; // do i need this?
	float xTextureCoord;
	float yTextureCoord;
	float xMaxTextureCoord;
	float yMaxTextureCoord;
	float xOffset;
	float yOffset;
	float sizeX;
	float sizeY;
	float xAdvance;

	Character() {}

	Character(int c_id, float c_x, float c_y, float c_xMax, float c_yMax, float c_xOffset, float c_yOffset, float c_xSize, float c_ySize, float c_xAdvance): id(c_id), xTextureCoord(c_x), yTextureCoord(c_y), xMaxTextureCoord(c_xMax),  yMaxTextureCoord(c_yMax), xOffset(c_xOffset), yOffset(c_yOffset), sizeX(c_xSize), sizeY(c_ySize), xAdvance(c_xAdvance)
	{}
};

struct Word {
	std::vector<Character> characters;
};

struct Line {
	std::vector<Word> words;
};


struct Font {
	std::unordered_map<int, Character> Char_map;
	unsigned int atlasTexture;
	float spaceWidth = 0.0f;

	Font();
	void Init(std::string fileName, std::string textureName);
};

class UI_text {
public:
	UI_text();
	void Init(std::string text, Font* font);

	float getSize(Word word);
	float getSize(Line line);

	void addVerticesForCharacter(Character& chara, float curX, float curY);
	void addVertexPos(float x, float y, float maxX, float maxY);
	void addTexCoords(float x, float y, float maxX, float maxY) ;


	std::vector<Word> WordsFromText(std::string f_text);
	std::vector<Line> LinesFromWords(std::vector<Word> words);

	void RecalculateText(std::string f_text);
	void Draw(Shader& shader);
	void ImGuiDraw();


public:

	std::vector<float> vertexCoord;
	std::vector<float> textureCoord;


	bool isCentered = false;
	float fontSize = 100.0f;
	unsigned int textVAO, vertexVBO, textureVBO;

	std::string text;

	Font* font;

	float width = 0.42f;
	float edge = 0.1f;
	float glyphWidth = 0.66f;

	glm::vec2 vPos = {400.0f, 200.0f};
	/*	glm::vec3 vPos = {0.0f, 0.0f, -18.0f};
		glm::vec3 vScale = {1.0f, 1.0f, 1.0f};
		glm::vec3 vRot = {0.0f, 90.0f, 0.0f};*/
	glm::vec3 color = {0.0f, 0.0f, 1.0f};
	float angle;

	float lineMaxWidth = 0.1f;
	float lineMaxHeight = 0.2f;
};
