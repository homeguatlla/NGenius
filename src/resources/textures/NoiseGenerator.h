#pragma once
#include <glm/glm.hpp>

#include <vector>

class NoiseGenerator
{
	static const int OCTAVES;
	static const float LACUNARITY;
	static const float GAIN;

public:
	NoiseGenerator();

	float FBM(glm::vec2 st);
	
	~NoiseGenerator();

private:
	float Noise(glm::vec2 st);
	float Random(glm::vec2 st);

	int mSize;
};

