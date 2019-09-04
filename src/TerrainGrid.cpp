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

void TerrainGrid::GenerateNormalsAndTangentsRectangular(int side, std::vector<glm::vec3>& points, std::vector<glm::vec2>& uv, std::vector<unsigned int>& indices, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents)
{
	struct Face
	{
		Face(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3, glm::vec3& n, glm::vec3& t) {
			vertex[0] = v1;
			vertex[1] = v2;
			vertex[2] = v3;
			normal = n;
			tangent = t;
		}
		glm::vec3 vertex[3];
		glm::vec3 normal;
		glm::vec3 tangent;
	};

	std::vector<Face> faces;
	int numFaces = indices.size() / 3;
	int i = 0;
	while(numFaces > 0)
	{
		int index1 = i;
		int index2 = i + 1;
		int index3 = i + 2;

		glm::vec3 vertex1 = points[indices[index1]];
		glm::vec3 vertex2 = points[indices[index2]];
		glm::vec3 vertex3 = points[indices[index3]];

		glm::vec3 normal = CalculateTriangleNormalFromVertex(vertex1, vertex2, vertex3);

		glm::vec2 uv1 = uv[indices[index1]];
		glm::vec2 uv2 = uv[indices[index2]];
		glm::vec2 uv3 = uv[indices[index3]];
		glm::vec3 tangent = CalculateTriangleTangentFromVertex(vertex1, vertex2, vertex3, uv1, uv2, uv3);
		faces.push_back(Face(vertex1, vertex2, vertex3, normal, tangent));

		i +=3;
		numFaces--;
	}

	for (unsigned int i = 0; i < points.size(); ++i)
	{
		glm::vec3 normal(0.0f);
		glm::vec3 tangent(0.0f);
		std::vector<int> vertexFaces;
		CalculateFacesVertex(i, side, vertexFaces);
		
		for (unsigned int face = 0; face < vertexFaces.size(); ++face)
		{
			normal += faces[face].normal;
			tangent += faces[face].tangent;
		}
		normal = glm::normalize(normal);
		tangent = glm::normalize(tangent);

		normals.push_back(normal);
		tangents.push_back(tangent);
	}
}

glm::vec3 TerrainGrid::CalculateTriangleTangentFromVertex(glm::vec3& vertex1, glm::vec3& vertex2, glm::vec3& vertex3, glm::vec2& uv1, glm::vec2& uv2, glm::vec2& uv3)
{
	glm::vec3 edge1 = vertex2 - vertex1;
	glm::vec3 edge2 = vertex3 - vertex1;

	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);
	return tangent;
}

void TerrainGrid::CalculateFacesVertex(int vertexIndex, int side, std::vector<int>& faces)
{
	int rows = vertexIndex / side;
	int columns = vertexIndex % side;
	int numFacePoint = rows * (side - 1) * 2 + columns * 2;

	if (columns == side - 1)
	{
		if (rows < side - 1)
		{
			faces.push_back(numFacePoint - 1);
			faces.push_back(numFacePoint - 2);
		}
		if (rows > 0)
		{
			faces.push_back(numFacePoint - 1 - (side - 1) * 2);
		}
	}
	else
	{
		int face = numFacePoint - (side - 1) * 2 - 1;
		int numFacesToAdd = 3;
		if (columns == 0)
		{
			face++;
			numFacesToAdd = 2;
			if (rows < side - 1)
			{
				faces.push_back(numFacePoint);
			}
		}

		for (int j = 0; j < numFacesToAdd; ++j)
		{
			if (face >= 0)
			{
				faces.push_back(face);
			}
			face++;
		}

		if (columns > 0 && rows < side -1)
		{
			face = numFacePoint - 2;
			for (int j = 0; j < 3; ++j)
			{
				if (face >= 0)
				{
					faces.push_back(face);
				}
				face++;
			}
		}
	}
}

glm::vec3 TerrainGrid::CalculateTriangleNormalFromVertex(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	//This assumes that A->B->C is a counter-clockwise ordering
	glm::vec3 n = glm::cross(b - a, c - a);
	
	return glm::normalize(n);
}

void TerrainGrid::GeneratePointsAngular(std::vector<glm::vec3>& points, int side, float scale, float angle)
{

}