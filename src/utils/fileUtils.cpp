#include "mypch.h"
#include "utils/fileUtils.h"
#include "core/application.h"


std::string OpenFile(nfdfilteritem_t filterItem[], int numFilters)
{


	nfdchar_t *outPath;
	nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, numFilters, NULL);
	if (result == NFD_OKAY)
	{
		return std::string(outPath);
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
		return std::string(outPath);
		NFD_FreePath(outPath);
	}
	else if (result == NFD_ERROR)
	{
		Log("Error: " << NFD_GetError());
	}
	return std::string();

}