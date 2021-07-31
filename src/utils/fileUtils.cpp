#include "mypch.h"
#include "utils/fileUtils.h"
#include "core/application.h"


std::string OpenFile(nfdfilteritem_t filterItem[], int numFilters)
{


    nfdchar_t *outPath;
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, numFilters, NULL);
    if (result == NFD_OKAY)
    {
        std::string outPathString(outPath);
#ifndef __linux__
        std::replace(outPathString.begin(), outPathString.end(), '\\', '/');
#endif
        return outPathString;
        NFD_FreePath(outPath);
    }
    else if (result == NFD_ERROR)
    {
        Log("Error: " << NFD_GetError());
    }
    return std::string();
}

std::string SaveFile(nfdfilteritem_t filterItem[], int numFilters)
{
    nfdchar_t *outPath;
    nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, numFilters, NULL, NULL);
    if (result == NFD_OKAY)
    {
        std::string outPathString(outPath);
#ifndef __linux__
        std::replace(outPathString.begin(), outPathString.end(), '\\', '/');
#endif
        return outPathString;
        NFD_FreePath(outPath);
    }
    else if (result == NFD_ERROR)
    {
        Log("Error: " << NFD_GetError());
    }
    return std::string();

}
