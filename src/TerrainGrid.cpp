#include "stdafx.h"
#include "TerrainGrid.h"
#include "assert.h"


TerrainGrid::TerrainGrid()
{
}

TerrainGrid::~TerrainGrid()
{
}

void TerrainGrid::GeneratePointsRectangular(std::vector<glm::vec3>& points, std::vector<glm::vec2>& uv, int side, float scale, int holeSize, bool textureCoords)
{
	points.clear();

	mSide = side;
	float min = -scale * 0.5f;
	float max = scale * 0.5f;
	float inc = scale / (side - 1);
	float size = max - min;

	//assert(holeSize >= 0 && holeSize <= side - 2); 
	//assert( (side - holeSize) % 2 == 0); //if side = 9, holeSize = { 7, 5, 3, 1}

	if (textureCoords)
	{
		for (float x = 0; x < mSide; ++x)
		{
			for (float z = 0; z < mSide; ++z)
			{
				glm::vec3 v(min + x * inc, 0.0f, min + z * inc);
				points.push_back(v);

				glm::vec2 t(x * inc / size, z * inc / size);
				uv.push_back(t);
			}
		}
	}
	else
	{
		for (float x = min; x <= max; x += inc)
		{
			for (float z = min; z <= max; z += inc)
			{
				glm::vec3 v(x, 0.0f, z);
				points.push_back(v);
			}
		}
	}
}

void TerrainGrid::GenerateIndicesRectangular(std::vector<unsigned int>& indices)
{
	indices.clear();

	for (unsigned int i = 0; i < mSide - 1; i++)
	{
		for (unsigned int j = 0; j < mSide - 1; j++)
		{
			int index = i + j * mSide;
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + mSide);

			indices.push_back(index + mSide);
			indices.push_back(index + 1);
			indices.push_back(index + mSide + 1);
		}
	}
}

void TerrainGrid::GeneratePointsAngular(std::vector<glm::vec3>& points, int side, float scale, float angle)
{

}