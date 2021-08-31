#include "mypch.h"
#include "core/tag.h"

std::vector<std::string> TagManager::tagList = {""};

int TagManager::GetIndexOfTag(const std::string& tagName) {
    const int n = tagList.size();
    for(int i = 0; i < n; i++)	{
        if(tagList[i] == tagName) {
            return i;
        }
    }
    return -1;
}
int TagManager::GetIndexOfTagAndMakeIfNone(const std::string& tagName) {
    const int n = tagList.size();
    for(int i = 0; i < n; i++)	{
        if(tagList[i] == tagName) {
            return i;
        }
    }
    tagList.push_back(tagName);
    return n;
}

