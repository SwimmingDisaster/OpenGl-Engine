#include "core/other/random.h"

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;


void Random::Init()
{
    srand((unsigned int)time(NULL));
    s_RandomEngine.seed((unsigned int)std::random_device()());
}


int Random::Int() {
    return s_Distribution(s_RandomEngine);
}

int Random::Int(float maxVal) {
    return (int)(Random::Float() * maxVal);
}

int Random::Int(float minVal, float maxVal) {
    return round((Random::Float() * (maxVal - minVal)) + minVal);
}

float Random::Float()
{
    return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint64_t>::max();
}

float Random::Float(float maxVal) {
    return Random::Float() * maxVal;
}

float Random::Float(float minVal, float maxVal) {
    return Random::Float() * (maxVal - minVal) + minVal;
}
