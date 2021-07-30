#pragma once
#include "mypch.h"
#include "core/shader.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

#include "assets/transform.h"

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


class Font : public Component {
    REGISTER(Font);
public:
    void Show() override;
    void Start() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;

public:
    std::unordered_map<int, Character> Char_map;
    unsigned int atlasTexture;
    float spaceWidth = 0.0f;

public:
    std::string fileName;
    std::string textureName;


    Font();
#ifdef SHOW_DELETED
public:
    virtual ~Font();
#else
public:
    virtual ~Font() {};
#endif
};

class Text : public Component {
    REGISTER(Text);
public:
    void Start() override;
    void Draw() override;
    void Show() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;

public:
    void Init(std::string text, Font* font);

    float getSize(Word word);
    float getSize(Line line);

    void addVerticesForCharacter(Character& chara, float curX, float curY);
    void addVertexPos(float x, float y, float maxX, float maxY);
    void addTexCoords(float x, float y, float maxX, float maxY) ;

    std::vector<Word> WordsFromText(std::string f_text);
    std::vector<Line> LinesFromWords(std::vector<Word> words);

    void RecalculateText(std::string& f_text);
	
public:
    Text();
#ifdef SHOW_DELETED
public:
    virtual ~Text();
#else
public:
    virtual ~Text() {};
#endif

public:
    std::string text;

    bool isCentered = false;

    float fontSize = 100.0f;
    float glyphWidth = 0.66f;
    float width = 0.42f;
    float edge = 0.1f;

    float lineMaxWidth = 0.1f;
    float lineMaxHeight = 0.2f;

    float angle = 0.0f;

    //glm::vec2 vPos = {400.0f, 200.0f};
    glm::vec3 color = {0.0f, 0.0f, 1.0f};

	std::string shaderName;

private:
	std::shared_ptr<Transform> transform;
    std::shared_ptr<Font> font;
	std::shared_ptr<Shader> shader;

    unsigned int textVAO, vertexVBO, textureVBO;
    std::vector<float> vertexCoord;
    std::vector<float> textureCoord;
};
