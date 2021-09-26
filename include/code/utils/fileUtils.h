#pragma once
#include "nfd/nfd.h"  // for nfdfilteritem_t
#include <string>    // for string


std::string OpenFile(nfdfilteritem_t filterItem[], int numFilters);
std::string SaveFile(nfdfilteritem_t filterItem[], int numFilters);
