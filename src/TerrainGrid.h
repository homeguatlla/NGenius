#pragma once
#include <vector>
#include <glm/glm.hpp>

class TerrainGrid
{
public:
	TerrainGrid();
	virtual ~TerrainGrid();

	//calcular hueco interior, y generar triangulos


	void GeneratePointsRectangular(std::vector<glm::vec3>& points, std::vector<glm::vec2>& uv, int side, float scale = 1.0f, int holeSize = 0, bool textureCoords = false);
	void GenerateIndicesRectangular(std::vector<unsigned int>& indices);
	void GeneratePointsAngular(std::vector<glm::vec3>& points, int side, float scale = 1.0f, float angle = 60.0f);

private:
	unsigned int mSide;
};

