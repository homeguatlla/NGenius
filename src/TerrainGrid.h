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
	void GenerateNormalsAndTangentsRectangular(int side, std::vector<glm::vec3>& points, std::vector<glm::vec2>& uv, std::vector<unsigned int>& indices, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents);
	
	void GeneratePointsAngular(std::vector<glm::vec3>& points, int side, float scale = 1.0f, float angle = 60.0f);
	glm::vec3 CalculateTriangleNormalFromVertex(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

private:
	void CalculateFacesVertex(int vertexIndex, int side, std::vector<int>& faces);
	glm::vec3 CalculateTriangleTangentFromVertex(glm::vec3& vertex1, glm::vec3& vertex2, glm::vec3& vertex3, glm::vec2& uv1, glm::vec2& uv2, glm::vec2& uv3);

	unsigned int mSide;
};

