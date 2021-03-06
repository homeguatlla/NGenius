#include "stdafx.h"
#include "Terrain.h"
#include "../renderers/IndicesRenderer.h"
#include "../../TerrainGrid.h"
#include "../textures/ITexture.h"
#include "../textures/TextureGenerator.h"
#include "../textures/Texture.h"
#include "../models/Mesh.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include <ctime>

#include "../systems/renderSystem/RenderSystem.h"

#include "../../utils/serializer/IDeserializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../../utils/Log.h"

#include "../Memory.h"

#include <glm/gtc/matrix_transform.hpp>

Terrain::Terrain(Transformation* transformation) :
	BaseGameEntity<Terrain>(transformation),
	mHeightmap(nullptr),
	mScale(1.0f)
{
}

Terrain::Terrain(Transformation* transformation, IMaterial* material, Texture* heightmap, float scale) :
	BaseGameEntity<Terrain>(transformation),
mHeightmap(nullptr),
mScale(scale)
{
	CreateTerrain(material, heightmap);
}

Terrain::~Terrain()
{
	delete mModel;
}

void Terrain::SetScale(float scale)
{
	mScale = scale;
	mMaterialEffectFloat->SetValue(mScale);
}

void Terrain::Build(NGenius* engine)
{
	IMaterial* material = engine->GetMaterial(mMaterialName);
	ITexture* heighmap = static_cast<Texture*>(engine->GetTexture(mHeightmapName));

	if (!mModelName.empty())
	{
		mModel = engine->GetModel(mModelName);
		if (mModel == nullptr)
		{
			Log(Log::LOG_ERROR) << "Couldn't found terrain model " << mModelName << "\n";
		}
	}
	CreateTerrain(material, heighmap);
}

void Terrain::ReadFrom(core::utils::IDeserializer* source)
{
	BaseGameEntity::ReadFrom(source);

	source->ReadParameter("heighmap_texture", mHeightmapName);
	source->ReadParameter("scale", &mScale);
	source->ReadParameter("model", mModelName);
}

std::shared_ptr<IGameEntity> Terrain::DoCreate()
{
	return std::make_shared<Terrain>();
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

	if (IsPointInside(point))
	{
		float inc = mGridSize / (mNumVertexsSide - 1);

		float X1 = point.x;
		float X2 = point.x + inc;
		float Z1 = point.y;
		float Z2 = point.y + inc;

		bool isLeft = glm::sign((X2 - X1) * (point.y - Z2) - (Z1 - Z2) * (point.x - X1));
		X1 += mGridSize * 0.5f;
		X2 += mGridSize * 0.5f;
		Z1 += mGridSize * 0.5f;
		Z2 += mGridSize * 0.5f;

		X1 /= inc;
		X2 /= inc;
		Z1 /= inc;
		Z2 /= inc;

		X1 = glm::floor(X1);
		X2 = glm::floor(X2);
		Z1 = glm::floor(Z1);
		Z2 = glm::floor(Z2);

		glm::vec3 vertex1, vertex2, vertex3;

		if (isLeft)
		{
			vertex1 = mVertexs[static_cast<unsigned int>(Z1 + X2 * (mNumVertexsSide))];
			vertex2 = mVertexs[static_cast<unsigned int>(Z2 + X1 * (mNumVertexsSide))];
			vertex3 = mVertexs[static_cast<unsigned int>(Z2 + X2 * (mNumVertexsSide))];
		}
		else
		{
			vertex1 = mVertexs[static_cast<unsigned int>(Z1 + X1 * (mNumVertexsSide))];
			vertex2 = mVertexs[static_cast<unsigned int>(Z2 + X1 * (mNumVertexsSide))];
			vertex3 = mVertexs[static_cast<unsigned int>(Z1 + X2 * (mNumVertexsSide))];
		}
		height = CalculateBarryCenter(vertex1, vertex2, vertex3, point);
	}

	return height;
}

float Terrain::GetHeightFromColor(glm::vec2 point) const
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

	return height;
}

void Terrain::CalculateY()
{
	//TODO este c�lculo se podr�a hacer dentro del grid. Aunque el grid no conoce de alturas pues vienen en el heighmap
	//igual est� bien como est� ahora aunque penalice el performance.
	float gridSquareSizeInTexture = static_cast<float>(mHeightmap->GetWidth() / (mNumVertexsSide - 1));
	float conversionToTextureValue = gridSquareSizeInTexture / mHeightmap->GetWidth();
	float gridSquareSize = mGridSize / (mNumVertexsSide - 1);

	for (unsigned int i = 0; i < mVertexs.size(); i++)
	{
		float X = mVertexs[i].x + mGridSize * 0.5f;
		float Z = mVertexs[i].z + mGridSize * 0.5f;

		int gridX = static_cast<int>(glm::floor(X / gridSquareSize));
		int gridZ = static_cast<int>(glm::floor(Z / gridSquareSize));
		float colorX = gridX * conversionToTextureValue;
		float colorZ = gridZ * conversionToTextureValue;
		glm::vec4 color = mHeightmap->GetColor(glm::vec2(colorX, colorZ)) / 256.0f;

		mVertexs[i].y = color.a * mScale;
	}
}

float Terrain::CalculateBarryCenter(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec2& point) const
{ 
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x -p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (point.x - p3.x) + (p3.x - p2.x) * (point.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (point.x - p3.x) + (p1.x - p3.x) * (point.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 *p3.y;
}

void Terrain::CreateTerrain(IMaterial* material, ITexture* heighmap)
{
	//better generate texture high resolution.
	//With 256 num vertexs side seems not so much different than with 1024. With 1024 there are more bumps and in fact is worst and the performance downs 50%.
	//TextureGenerator tg;
	//tg.Generate("data/terrain_heightmap_256.png", mNumVertexsSide, scale, true);
	//tg.GenerateOnlyBlendmap("data/terrain_blendmap_1024.png", mNumVertexsSide);
	//return;

	assert(material != nullptr);

	srand(static_cast<unsigned int>(time(NULL)));

	if (mModel == nullptr)
	{
		assert(heighmap != nullptr);

		std::vector<glm::vec2> uv;
		std::vector<unsigned int> indices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;

		mGridSize = 120;
		mNumVertexsSide = 256;
		TerrainGrid terrainGrid;
		terrainGrid.GeneratePointsRectangular(mVertexs, uv, mNumVertexsSide, mGridSize, 0, true);
		terrainGrid.GenerateIndicesRectangular(indices);
		
		mHeightmap = static_cast<Texture*>(heighmap);
		CalculateY();
		terrainGrid.GenerateNormalsAndTangentsRectangular(mNumVertexsSide, mVertexs, uv, indices, normals, tangents);

		Mesh* mesh = new Mesh(mVertexs, uv, indices, normals, tangents);
		mModel = DBG_NEW Model("terrain", mesh);
	}

	SetRenderer(new IndicesRenderer(mModel, material));
	GetRenderer()->SetLayer(IRenderer::LAYER_TERRAIN);

	mMaterialEffectFloat = material->GetEffect<MaterialEffectFloat>();
	assert(mMaterialEffectFloat != nullptr);
}

void Terrain::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}
