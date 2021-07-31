#pragma once
#include "mypch.h"

class TextureManager {
public:
	static std::vector<std::string> textureList;
	//for some stupid reason there is a runtime error if this 
	//uses 'unsigned int' instead of normal 'int'
	//it is possible to use 'unsigned int' tho
	//if this is a map instead of unordered_map
	static std::unordered_map<std::string, int> textureMap;
};


class TextureInfo {
public:
	std::string name;
};

unsigned int loadTexture(char const * path);

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
