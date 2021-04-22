#include "mypch.h"
#include "utils/fileUtils.h"
#include "core/application.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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

	// OPENFILENAMEA ofn;
	// CHAR szFile[260] = {0};
	// const CHAR currentDir[256] = {0};
	// ZeroMemory(&ofn, sizeof(OPENFILENAME));
	// ofn.lStructSize = sizeof(OPENFILENAME);
	// ofn.hwndOwner = glfwGetWin32Window(Application::window);
	// ofn.lpstrFile = szFile;
	// ofn.nMaxFile = sizeof(szFile);
	// ofn.lpstrFilter = filter;
	// ofn.nFilterIndex = 1;
	// ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// if (GetOpenFileNameA(&ofn) == TRUE)
	// 	return ofn.lpstrFile;
	// return std::string();
}
std::string SaveFile(const char *filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = {0};
	const CHAR currentDir[256] = {0};
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window(Application::window);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;
	return std::string();
}