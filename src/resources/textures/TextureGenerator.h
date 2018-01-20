#pragma once

#include <glm/glm.hpp>
#include "NoiseGenerator.h"

class TextureGenerator
{
public:
	TextureGenerator();
	~TextureGenerator();

	void Generate(std::string filename, int size = 256, float heightScale = 1.0f, bool generateNormals = false);
	void GenerateOnlyBlendmap(std::string filename, int size);
	
private:
	void GenerateHeighmap(int size, std::vector<unsigned char>& heightmap, int length);
	void GenerateNormals(int size, std::vector<unsigned char>& heightmap, int length, float heightScale);
	void GenerateBlendmap(int size, std::vector<unsigned char>& heightmap, std::vector<unsigned char>& blendmap, int length, float minHeight, float maxHeight);
	float CalculateHeight(NoiseGenerator& ng, glm::vec2 p);
	glm::vec3 CalculateNormal(int i, int j, std::vector<unsigned char>& heightmap, int length, float heightScale);
	glm::vec3 CalculateNormalByTriangles(int i, int j, std::vector<unsigned char>& heightmap, int length, float heightScale);
	glm::vec4 CalculateBlendColor(unsigned int height, float minHeight, float maxHeight);
	void SavePNG(std::string& filename, int size, std::vector<unsigned char>& data);
	void ModifyToIsland(std::vector<unsigned char>& heightmap, int size, float radius, float min);

	bool IsInterior(int i, int j, int size);
};