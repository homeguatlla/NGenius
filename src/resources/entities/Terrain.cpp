#include "stdafx.h"
#include "Terrain.h"
#include "../textures/ITexture.h"
#include "../../renderer/TerrainRenderer.h"
#include "../../TerrainGrid.h"
#include "../textures/TextureGenerator.h"
#include "../textures/Texture.h"
#include "../textures/TextureArray.h"
#include "../camera/ICamera.h"
#include <ctime>

#include <glm/gtc/matrix_transform.hpp>

Terrain::Terrain(Transformation* transformation, IShaderProgram* shader, const Texture* heightmap, const Texture* blendmap, const TextureArray* textureArray, const Texture* shadowmap, const ICamera* shadowCamera, const Light* light, float scale) :
GameEntity(transformation),
mHeightmap(heightmap),
mShadowmap(shadowmap),
mShadowCamera(shadowCamera),
mScale(scale),
mIsFlat(false)
{
	SetRenderer(new TerrainRenderer(shader,
									heightmap,
									blendmap,
									textureArray,
									light,
									scale));
	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> indices;
	TerrainGrid terrainGrid;

	mNumVertexsSide = 256;

	//better generate texture high resolution.
	//With 256 num vertexs side seems not so much different than with 1024. With 1024 there are more bumps and in fact is worst and the performance downs 50%.
	//TextureGenerator tg;
	//tg.Generate("data/terrain_heightmap_256.png", mNumVertexsSide, scale, true);
	//tg.GenerateOnlyBlendmap("data/terrain_blendmap_1024.png", mNumVertexsSide);
	//return;
	

	srand(time(NULL));

	mGridSize = 65;
	
	terrainGrid.GeneratePointsRectangular(vertexs, uv, mNumVertexsSide, mGridSize, 0, true);
	terrainGrid.GenerateIndicesRectangular(indices);

	GetRenderer()->SetVertexs(vertexs);
	static_cast<TerrainRenderer*>(GetRenderer())->SetTextureCoords(uv);
	GetRenderer()->SetIndexes(indices);

	//shadow stuff
	static_cast<TerrainRenderer*>(GetRenderer())->SetTextureShadowMap(mShadowmap);
	SetShadowCamera(shadowCamera);
}


Terrain::~Terrain()
{
}



void Terrain::SetFlat(bool isFlat)
{
	mIsFlat = isFlat;
}

void Terrain::SetShadowCamera(const ICamera* camera)
{
	mShadowCamera = camera;

	Transformation transformation(mShadowCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(1.0f));
	glm::mat4& matrix = mShadowCamera->GetProjectionMatrix() * const_cast<ICamera*>(mShadowCamera)->GetViewMatrix();// *transformation.GetModelMatrix();

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	static_cast<TerrainRenderer*>(GetRenderer())->SetShadowMapMatrix(biasMatrix * matrix);
}

bool Terrain::IsPointInside(glm::vec2 point) const
{
	float X = point.x + mGridSize * 0.5f;
	float Z = point.y + mGridSize * 0.5f;

	return X >= 0.0f && X < mGridSize && Z >= 0.0f && Z < mGridSize;
}

float Terrain::GetHeight(glm::vec2 point) const
{
	float height = -std::numeric_limits<float>::infinity();

	float X = point.x + mGridSize * 0.5f;
	float Z = point.y + mGridSize * 0.5f;

	float gridSquareSize = mGridSize / (mNumVertexsSide - 1);
	int gridX = static_cast<int>(glm::floor(X / gridSquareSize));
	int gridZ = static_cast<int>(glm::floor(Z / gridSquareSize));

	if (gridX >= 0 && gridX < mNumVertexsSide && gridZ >= 0 && gridZ < mNumVertexsSide)
	{
		float xCoord = (X - gridSquareSize * (static_cast<int>(X / gridSquareSize))) / gridSquareSize;
		float zCoord = (Z - gridSquareSize * (static_cast<int>(Z / gridSquareSize))) / gridSquareSize;

		float gridSquareSizeInTexture = static_cast<float>(mHeightmap->GetWidth() / (mNumVertexsSide - 1));
		glm::vec3 vertex1, vertex2, vertex3;
		float conversionToTextureValue = gridSquareSizeInTexture / mHeightmap->GetWidth();
		float gridXNormalized = gridX * conversionToTextureValue;
		float gridX1Normalized = (gridX + 1) * conversionToTextureValue;
		float gridZNormalized = gridZ * conversionToTextureValue;
		float gridZ1Normalized = (gridZ + 1) * conversionToTextureValue;

		float scaleFactor = mScale / 256.0f;

		//one side of the triangle
		if (xCoord <= 1.0f - zCoord)
		{
			vertex1 = glm::vec3(0.0f, mHeightmap->GetColor(glm::vec2(gridXNormalized, gridZNormalized)).a * scaleFactor, 0.0f);
			vertex2 = glm::vec3(1.0f, mHeightmap->GetColor(glm::vec2(gridX1Normalized, gridZNormalized)).a * scaleFactor, 0.0f);
			vertex3 = glm::vec3(0.0f, mHeightmap->GetColor(glm::vec2(gridXNormalized, gridZ1Normalized)).a * scaleFactor, 1.0f);
		}
		else
		{
			vertex1 = glm::vec3(1.0f, mHeightmap->GetColor(glm::vec2(gridXNormalized, gridZNormalized)).a * scaleFactor, 0.0f);
			vertex2 = glm::vec3(1.0f, mHeightmap->GetColor(glm::vec2(gridX1Normalized, gridZ1Normalized)).a * scaleFactor, 1.0f);
			vertex3 = glm::vec3(0.0f, mHeightmap->GetColor(glm::vec2(gridXNormalized, gridZ1Normalized)).a * scaleFactor, 1.0f);
		}

		height = CalculateBarryCenter(vertex1, vertex2, vertex3, glm::vec2(xCoord, zCoord));
	}

	return mIsFlat ? 0.0f : height;
}

float Terrain::CalculateBarryCenter(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec2& point) const
{ 
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x -p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (point.x - p3.x) + (p3.x - p2.x) * (point.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (point.x - p3.x) + (p1.x - p3.x) * (point.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 *p3.y;
}
