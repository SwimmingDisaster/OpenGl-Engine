#include "mypch.h"
#include "core/layer.h"

std::array<std::string, 32> LayerManager::layerList;

int LayerManager::GetIndexOfLayer(const std::string& layerName) {
    const int n = layerList.size();
    for(int i = 0; i < n; i++)	{
        if(layerList[i] == layerName) {
            return i;
        }
    }
    return -1;
}
int LayerManager::GetIndexOfLayerAndMakeIfNone(const std::string& layerName) {
    const int n = layerList.size();
    for(int i = 0; i < n; i++)	{
        if(layerList[i] == layerName) {
            return i;
        }
    }
    for(int i = 0; i < n; i++) {
        if(layerList[i] == "") {
            layerList[i] = layerName;
            return i;
        }
    }
    return -1;
}

