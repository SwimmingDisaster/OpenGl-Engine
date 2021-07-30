#pragma once

#include "mypch.h"

class Random
{
public:
	static void Init();

	[[nodiscard]] static int Int();
	[[nodiscard]] static int Int(float maxVal);
	[[nodiscard]] static int Int(float minVal, float maxVal);

	[[nodiscard]] static float Float();
	[[nodiscard]] static float Float(float maxVal);
	[[nodiscard]] static float Float(float minVal, float maxVal);


private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

