#include "stdafx.h"
#include "NoiseGenerator.h"

using namespace glm;

const int NoiseGenerator::OCTAVES = 6;
const float NoiseGenerator::LACUNARITY = 2.0f;
const float NoiseGenerator::GAIN = 0.5f;

NoiseGenerator::NoiseGenerator()
{
}


NoiseGenerator::~NoiseGenerator()
{
}

float NoiseGenerator::Noise(glm::vec2 st)
{
	vec2 i = floor(st);
	vec2 f = fract(st);

	// Four corners in 2D of a tile
	float a = Random(i);
	float b = Random(i + vec2(1.0f, 0.0f));
	float c = Random(i + vec2(0.0f, 1.0f));
	float d = Random(i + vec2(1.0f, 1.0f));

	vec2 u = f * f * (3.0f - 2.0f * f);

	return static_cast<float>(mix(a, b, u.x) +
		(c - a)* u.y * (1.0 - u.x) +
		(d - b) * u.x * u.y);
}

float NoiseGenerator::FBM(glm::vec2 st)
{
	// Initial values
	float value = 0.0f;
	float amplitud = 0.6f;
	float frequency = 0.0f;
	
	//vec2 shift = vec2(100.0);
	//mat2 rot = mat2(cos(0.5), sin(0.5),
	//	-sin(0.5), cos(0.50));
	// Loop of octaves
	for (int i = 0; i < OCTAVES; i++) 
	{ 
		value += amplitud * Noise(st);
		//st = rot * st * LACUNARITY + shift;
		st *= LACUNARITY;
		amplitud *= GAIN;
	}

	return value;
}

float NoiseGenerator::Random(glm::vec2 st)
{
	return fract(sin(dot(st,
			vec2(12.9898f, 78.233f)))*
			43758.5453123f);
}