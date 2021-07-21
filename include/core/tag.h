#pragma once
#include "mypch.h"

class TagManager{
public:
	static std::vector<std::string> tagList;
public:
	static int GetIndexOfTag(const std::string& tagName);
	static int GetIndexOfTagAndMakeIfNone(const std::string& tagName);
};
