#include "mypch.h"
#include "assets/ui/text.h"
#include "core/shader.h"
#include "core/engineInfo.h"
#include "core/texture.h"
#include "boost/algorithm/string.hpp"


REGISTERIMPL(Font);

void Font::Start() {
    if(fileName == "" || textureName == "") {
        return;
    }
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
        Char_map[id] = Character(id, x, y, xMax, yMax, xOffset, yOffset, sizeX, sizeY, xAdvance);
    }

    atlasTexture = loadTexture(textureName.c_str());
    spaceWidth = Char_map[32].xAdvance;
}

void Font::Show() {
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.50f);
    ImGui::InputText("File name", &fileName, ImGuiInputTextFlags_CallbackResize);
    ImGui::SameLine();
    ImGui::PopItemWidth();
    if (ImGui::Button("Browse##1")) {
        fileName = OpenFile(NULL, 0);
    }
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.50f);
    ImGui::InputText("Texture name", &textureName, ImGuiInputTextFlags_CallbackResize);
    ImGui::SameLine();
    ImGui::PopItemWidth();
    if (ImGui::Button("Browse##2")) {
        textureName = OpenFile(NULL, 0);
    }
}


void Font::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "FileName" << YAML::Value << fileName;
    out << YAML::Key << "TextureName" << YAML::Value << textureName;

    out << YAML::EndMap;
}
void Font::Deserialize(const YAML::Node& data) {
    fileName = data["FileName"].as<std::string>();
    textureName = data["TextureName"].as<std::string>();
}

Font::Font() {
    name = "Font";
}
#ifdef SHOW_DELETED
Font::~Font() {
    Log("Deleted " << name);
}
#endif

REGISTERIMPL(Text);


std::vector<Word> Text::WordsFromText(std::string f_text) {
    std::vector<std::string> strs;

    boost::split(strs, f_text, boost::is_any_of("\t "), boost::token_compress_on);
    unsigned long numWords = strs.size();
    std::vector<Word> words(numWords);

    for (unsigned long i = 0; i < numWords; i++) {
        words[i].characters.reserve(strs[i].length());
        for (unsigned long j = 0; j < strs[i].length(); j++) {
            words[i].characters.emplace_back(font->Char_map[strs[i][j]]);
        }
    }
    return words;
}

std::vector<Line> Text::LinesFromWords(std::vector<Word> words) {
    std::vector<Line> lines(1);
    float lineSize = 0;
    unsigned long index = 0;
    for (unsigned long i = 0; i < words.size(); i++) {
        if (lineSize + getSize(words[i]) * fontSize < lineMaxWidth * EngineInfo::SCREEN_WIDTH) {
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

float Text::getSize(Word word) {
    float size = 0.0f;
    for (Character& c : word.characters) {
        size += c.xAdvance;
    }
    return size;
}

float Text::getSize(Line line) {
    float size = 0.0f;
    for (Word& w : line.words) {
        size += getSize(w);
    }
    size += (line.words.size() - 1) * font->spaceWidth;
    return size;
}


void Text::addVerticesForCharacter(Character& chara, float curX, float curY) {
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

void Text::addVertexPos(float x, float y, float maxX, float maxY) {
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

void Text::addTexCoords(float x, float y, float maxX, float maxY) {
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



void Text::RecalculateText(std::string& f_text) {

    vertexCoord.clear();
    textureCoord.clear();
    std::vector<Word> words = WordsFromText(f_text);
    std::vector<Line> lines = LinesFromWords(words);

    float lineMaxWidthMiddle = (lineMaxWidth / 2) * EngineInfo::SCREEN_WIDTH;

    float cursorX = -lineMaxWidthMiddle, cursorY = 0;
    cursorY -=  lines.size() * lineMaxHeight * fontSize / 2;
    for (Line& l : lines) {
        if (isCentered) {
            cursorX = ((lineMaxWidth * EngineInfo::SCREEN_WIDTH - getSize(l) * fontSize) / 2) - lineMaxWidthMiddle;
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

    if (vertexCoord.size() > 0 ) //IF THE STRING IS NOT EMPTY
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
void Text::Start() {
    transform = parentEntity->GetComponent<Transform>();
    font = parentEntity->GetComponent<Font>();
    shader = Shader::shaderMap[shaderName].get();
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &textureVBO);
    RecalculateText(text);
}

void Text::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "Text" << YAML::Value << text;
    out << YAML::Key << "IsCentered" << YAML::Value << (int)isCentered;
    out << YAML::Key << "FontSize" << YAML::Value << fontSize;
    out << YAML::Key << "GlyphWidth" << YAML::Value << glyphWidth;
    out << YAML::Key << "Width" << YAML::Value << width;
    out << YAML::Key << "Edge" << YAML::Value << edge;
    out << YAML::Key << "LineMaxWidth" << YAML::Value << lineMaxWidth;
    out << YAML::Key << "LineMaxHeight" << YAML::Value << lineMaxHeight;
    out << YAML::Key << "Angle" << YAML::Value << angle;
    out << YAML::Key << "Color" << YAML::Value << color;
    out << YAML::Key << "ShaderName" << YAML::Value << shaderName;

    out << YAML::EndMap;
}

void Text::Deserialize(const YAML::Node& data) {
    text = data["Text"].as<std::string>();
    isCentered = (bool)data["IsCentered"].as<int>();
    fontSize = data["FontSize"].as<float>();
    glyphWidth = data["GlyphWidth"].as<float>();
    width = data["Width"].as<float>();
    edge = data["Edge"].as<float>();
    lineMaxWidth = data["LineMaxWidth"].as<float>();
    lineMaxHeight = data["LineMaxHeight"].as<float>();
    angle = data["Angle"].as<float>();
    color = data["Color"].as<glm::vec3>();
    shaderName = data["ShaderName"].as<std::string>();

}


void Text::Draw() {


    shader->use();
    shader->setFloat("text", font->atlasTexture);
    shader->setVec3("textColor", color);
    shader->setVec2("position", {transform->position.x, transform->position.y});

    shader->setFloat("width", width);
    shader->setFloat("edge", edge);

    glm::mat4 proj = glm::ortho(0.0f, (float)EngineInfo::SCREEN_WIDTH, 0.0f, (float)EngineInfo::SCREEN_HEIGHT);
    float scale = EngineInfo::SCREEN_HEIGHT / 1080.0f;
    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
    shader->setMat4("projection", /*EngineInfo::projectionMatrix * EngineInfo::viewMatrix * */ proj * model);

    glm::mat4 rot = glm::translate(glm::mat4(1.0f), glm::vec3(transform->position.x, transform->position.y, 0.0f));
    rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0f));
    shader->setMat4("rotation", rot);
    if (vertexCoord.size() > 0 ) //IF THE STRING IS NOT EMPTY
    {
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font->atlasTexture);

        glBindVertexArray(textVAO);

        glEnableVertexAttribArray(0);
        //glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, vertexCoord.size() / 2);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
}

void Text::Show() {
    bool changed = false;
    changed |= ImGui::InputText("Text box", &text, ImGuiInputTextFlags_CallbackResize);
    /*changed |= ImGui::DragFloat3(("Position-").c_str(), glm::value_ptr(vPos));
      changed |= ImGui::DragFloat3(("Scale-").c_str(), glm::value_ptr(vScale));
      changed |= ImGui::DragFloat3(("Rotation-").c_str(), glm::value_ptr(vRot));*/
    ImGui::ColorEdit3("Color", glm::value_ptr(color));
    ImGui::DragFloat("Rotation",  &angle, 0.15f);
    changed |= ImGui::Checkbox("Is Centered", &isCentered);
    changed |= ImGui::DragFloat("Font size",  &fontSize);
    changed |= ImGui::DragFloat("line max width",  &lineMaxWidth, 0.001f);
    changed |= ImGui::DragFloat("line max height",  &lineMaxHeight, 0.01f);
    changed |= ImGui::DragFloat("Glyph Width",  &glyphWidth, 0.001f);
    ImGui::DragFloat("Width",  &width, 0.001f);
    ImGui::DragFloat("Edge",  &edge, 0.001f);

    ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
    if (changed) {
        RecalculateText(text);
    }
}

#ifdef SHOW_DELETED
Text::~Text() {
    Log("Deleted " << name);
}
#endif
Text::Text() {
    name = "Text";
}
