#include "stdafx.h"
#include "BuildingBuilder.h"
#include "../../resources/Transformation.h"
#include "../../resources/GameEntity.h"
#include "../../NGenius.h"

#include "../../resources/scene/GameScene.h"

#include "../../resources/components/PhysicsComponent.h"
#include "../../resources/components/CollisionComponent.h"
#include "../../resources/components/SpacePartitionComponent.h"

#include "../../resources/renderers/IRenderer.h"

#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../resources/materials/effects/MaterialEffectNormalTexture.h"
#include "../../resources/materials/effects/MaterialEffectDirectionalLightProperties.h"

BuildingBuilder::BuildingBuilder(GameScene* scene) : mScene(scene), mSize(4.0f, 4.0f, 4.0f)
{
	assert(scene != nullptr);
}


BuildingBuilder::~BuildingBuilder()
{
}

void BuildingBuilder::SetWallsMatrix(std::vector<std::vector<unsigned int>>& wallsMatrix)
{
	mWallsMatrix = wallsMatrix;
}

void BuildingBuilder::SetCentre(glm::vec3& centre)
{
	mCentre = centre;
}

void BuildingBuilder::Build()
{
	for (unsigned int i = 0; i < mWallsMatrix.size(); ++i)
	{
		for (unsigned int j = 0; j < mWallsMatrix[i].size(); ++j)
		{
			unsigned int piece = mWallsMatrix[i][j];
			std::vector<BuildWall> wallData = CreateBuildingData(piece, static_cast<float>(j), static_cast<float>(i));
			if (!wallData.empty())
			{
				mWalls.insert(mWalls.end(), wallData.begin(), wallData.end());
			}
		}
	}

	for (BuildWall wall : mWalls)
	{
		CreateWall(wall);
	}

	mWalls.clear();
	mWallsMatrix.clear();
}

std::vector<BuildingBuilder::BuildWall> BuildingBuilder::CreateBuildingData(unsigned int piece, float x, float z)
{
	std::vector<BuildWall> data; 
	std::bitset<8> binary = std::bitset<8>(piece);

	x *= -mSize.x;
	z *= -mSize.z;

	float yOffset = mSize.y * 0.5f;
	float wide = 0.10f;

	/*
	BuildWall buildData(
		glm::vec3(0.0f, yOffset, 0.0f) + mCentre * mSize,
		glm::vec3(0.0f),
		glm::vec3(mSize.x, mSize.y, mSize.z),
		std::string("floor1"),
		std::string("floor1_normal"), 1.0f);
	data.push_back(buildData);*/


	if (binary[0] == 1)
	{
		BuildWall buildData(
			glm::vec3(x, yOffset, z + 0.5f * mSize.z - mSize.z * wide * 0.5f) + mCentre * mSize,
			glm::vec3(0.0f), 
			glm::vec3(mSize.x, mSize.y, mSize.z * wide),
			std::string("floor1"), 
			std::string("floor1_normal"), 1.0f);

		if (binary[1] == 1 && binary[3] == 1)
		{
			buildData.scale.x = mSize.x - mSize.z * wide * 2.0f;
		}
		else if (binary[1] == 1)
		{
			buildData.scale.x = mSize.x - mSize.z * wide;
			buildData.position.x += mSize.z * wide * 0.5f;
		}
		else if (binary[3] == 1)
		{
			buildData.scale.x = mSize.x - mSize.z * wide;
			buildData.position.x -= mSize.z * wide * 0.5f;
		}

		data.push_back(buildData);
	}
	
	
	if (binary[3] == 1)
	{
		BuildWall buildData(
			glm::vec3(x + 0.5f * mSize.x - mSize.x * wide * 0.5f, yOffset, z) + mCentre * mSize,
			glm::vec3(0.0f),
			glm::vec3(mSize.x * wide, mSize.y, mSize.z),
			std::string("floor1"),
			std::string("floor1_normal"), 1.0f);
		data.push_back(buildData);
	}

	if (binary[2] == 1)
	{
		BuildWall buildData(
			glm::vec3(x, yOffset, z -0.5f * mSize.z + mSize.z * wide * 0.5f) + mCentre * mSize,
			glm::vec3(0.0f),
			glm::vec3(mSize.x, mSize.y, mSize.z * wide),
			std::string("floor1"),
			std::string("floor1_normal"), 1.0f);

		if (binary[1] == 1 && binary[3] == 1)
		{
			buildData.scale.x = mSize.x - mSize.z * wide * 2.0f;
		}
		else if (binary[1] == 1)
		{
			buildData.scale.x = mSize.x - mSize.z * wide;
			buildData.position.x += mSize.z * wide * 0.5f;
		}
		else if (binary[3] == 1)
		{
			buildData.scale.x = mSize.x - mSize.z * wide;
			buildData.position.x -= mSize.z * wide * 0.5f;
		}
		data.push_back(buildData);
	}

	if (binary[1] == 1)
	{
		BuildWall buildData(
			glm::vec3(x -0.5f * mSize.x + mSize.x * wide * 0.5f, yOffset, z) + mCentre * mSize,
			glm::vec3(0.0f),
			glm::vec3(mSize.x * wide, mSize.y, mSize.z),
			std::string("floor1"),
			std::string("floor1_normal"), 1.0f);
		data.push_back(buildData);
	}

	if (binary[4] == 1)
	{
		BuildWall buildData(
			glm::vec3(x, -mSize.y * wide * 0.5f, z) + mCentre * mSize,
			glm::vec3(0.0f),
			glm::vec3(mSize.x, mSize.y * wide, mSize.z),
			std::string("floor1"),
			std::string("floor1_normal"), 1.0f);
		data.push_back(buildData);
	}

	if (binary[5] == 1)
	{
		BuildWall buildData(
			glm::vec3(x, mSize.y + mSize.y * wide * 0.5f, z) + mCentre * mSize,
			glm::vec3(0.0f),
			glm::vec3(mSize.x, mSize.y * wide, mSize.z),
			std::string("floor1"),
			std::string("floor1_normal"), 1.0f);
		data.push_back(buildData);
	}

	return data;
}

void BuildingBuilder::CreateWall(BuildWall& wallData)
{
	NGenius* engine = &NGenius::GetInstance();

	Transformation* transformation = new Transformation(wallData.position, wallData.rotation, wallData.scale * 0.5f);

	std::string modelName = "cubeTexture";
	GameEntity* wall = engine->CreateGameEntityFromModelAndTextures(
		modelName,
		transformation,
		wallData.textureName,
		wallData.normalTextureName,
		0.0f);
	if (wall != nullptr)
	{
		wall->RemoveComponent<PhysicsComponent>();
		wall->RemoveComponent<CollisionComponent>();
		//wall->RemoveComponent<SpacePartitionComponent>();
		//wall->GetRenderer()->GetMaterial()->RemoveEffect<MaterialEffectDirectionalLightProperties>();
		IMaterial* material = wall->GetRenderer()->GetMaterial();
		MaterialEffectDiffuseTexture* diffuseEffect = material->GetEffect<MaterialEffectDiffuseTexture>();
		diffuseEffect->SetTile(wallData.textureTile);
		MaterialEffectNormalTexture* normalEffect = material->GetEffect<MaterialEffectNormalTexture>();
		if (normalEffect == nullptr)
		{
			ITexture* normalTexture = engine->GetTexture(wallData.normalTextureName);
			material->AddEffect(new MaterialEffectNormalTexture(normalTexture, wallData.textureTile));
		}
		else
		{
			normalEffect->SetTile(wallData.textureTile);
		}
		mScene->AddEntity(wall);
	}
}