#pragma once
#include "mypch.h"

class LayerManager{
public:
	static std::array<std::string, 32> layerList;
public:
	static int GetIndexOfLayer(const std::string& layerName);
	static int GetIndexOfLayerAndMakeIfNone(const std::string& layerName);
};
