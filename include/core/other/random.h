#pragma once

#include "mypch.h"

class Random
{
public:
	static void Init();

	static int Int();
	static int Int(float maxVal);
	static int Int(float minVal, float maxVal);

	static float Float();
	static float Float(float maxVal);
	static float Float(float minVal, float maxVal);


private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

