#include "stdafx.h"
#include "EntitiesPatch.h"
#include "../entities/Terrain.h"
#include "../BaseGameEntity.h"
#include "../entities/GameEntity.h"
#include "../Memory.h"

#include <iostream>


EntitiesPatch::EntitiesPatch(Transformation* transformation, const Terrain* terrain, float heightMin, float heightMax, float wide, float length, float density, bool isIntersectionAllowed) :
	BaseGameEntity(transformation),
	mTerrain(terrain),
	mHeightMin(heightMin),
	mHeightMax(heightMax),
	mWide(wide),
	mLength(length),
	mDensity(density),
	mIsIntersectionAllowed(isIntersectionAllowed)
{
}

IGameEntity* EntitiesPatch::DoCreate()
{
	return DBG_NEW EntitiesPatch();
}

void EntitiesPatch::Build(NGenius* engine)
{
	mTerrain = static_cast<Terrain*>(engine->GetGameEntity("terrain"));
	assert(mTerrain != nullptr);

	CreateEntities(engine);
}

void EntitiesPatch::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}

void EntitiesPatch::CreateEntities(NGenius* engine)
{
	FillEntitiesRadiusVector(engine);
	int numMaxEntitiesFittingArea = CalculateNumBiggerEntitiesFitArea();
	int numEntitiesWithDensity = static_cast<int>(numMaxEntitiesFittingArea * mDensity / 100);
	SpawnEntities(engine, numEntitiesWithDensity);
}

bool EntitiesPatch::CanPlaceEntityOnPoint(IGameEntity* entity, glm::vec3& point, std::vector<std::pair<glm::vec3, float>>& pointsWithEntityOver)
{
	if (!mIsIntersectionAllowed)
	{
		glm::vec2 size = entity->GetRenderer()->GetAABB().GetSize();
		float radius = size.x > size.y ? size.x : size.y;

		for (auto pair : pointsWithEntityOver)
		{
			float distanceBetweenPoints = glm::length(pair.first - point);
			if (distanceBetweenPoints < radius + pair.second)
			{
				return false;
			}
		}

		pointsWithEntityOver.push_back(std::pair<glm::vec3, float>(point, radius));
	}

	return true;
}

bool EntitiesPatch::FillWithRandomPoint(glm::vec3& point)
{
	bool found = false;
	int counter = 100;
	while (!found && counter > 0)
	{
		point.x = -mWide * 0.5f + (rand() % 1000) * (mWide / 1000.0f);
		point.z = -mLength * 0.5f + (rand() % 1000) * (mLength / 1000.0f);
		point = GetTransformation()->GetModelMatrix() * glm::vec4(point, 1.0f);

		point.y = mTerrain->GetHeight(glm::vec2(point.x, point.z));
		if (point.y >= mHeightMin && point.y <= mHeightMax)
		{
			found = true;
		}
		counter--;
	}

	return found;
}

int EntitiesPatch::CalculateNumBiggerEntitiesFitArea()
{
	float area = mWide * mLength;
	float radius = std::numeric_limits<float>::min();

	for (auto entity : mEntities)
	{
		if (entity.second > radius)
		{
			radius = entity.second;
		}
	}

	return static_cast<int>(area / (radius * 4.0f)); // a bit more of diameter instead of 2, x 3
}

void EntitiesPatch::FillEntitiesRadiusVector(NGenius* engine)
{
	for (auto model : mModelsName)
	{
		IGameEntity* entity = engine->GetGameEntity(model.first);
		if (entity != nullptr)
		{
			glm::vec2 size = entity->GetRenderer()->GetAABB().GetSize();
			float radius = size.x > size.y ? size.x : size.y;

			mEntities.push_back(std::pair<IGameEntity*, float>(entity, radius));
		}
	}
}

void EntitiesPatch::SpawnEntities(NGenius* engine, int numEntities)
{
	std::vector<std::pair<glm::vec3, float>> pointsWithRadiusAlreadyPlaced;

	for (int i = 0; i < mEntities.size(); ++i)
	{
		auto entityPair = mEntities[i];
		int numModelsToPlace = static_cast<int>(mModelsName[i].second * numEntities / 100);
		bool filled = true;
		while (filled && numModelsToPlace > 0)
		{
			glm::vec3 point;
			filled = FillWithRandomPoint(point);
			if (filled)
			{
				bool canPlaceIt = CanPlaceEntityOnPoint(entityPair.first, point, pointsWithRadiusAlreadyPlaced);

				if (canPlaceIt)
				{
					BaseGameEntity<GameEntity>* clone = static_cast<BaseGameEntity<GameEntity>*>(entityPair.first);
					IGameEntity* newEntity = clone->Clone();
					if (newEntity != nullptr)
					{
						newEntity->GetTransformation()->SetPosition(point);
						float rotationY = static_cast<float>(rand() % 360);
						newEntity->GetTransformation()->SetRotation(glm::vec3(0.0f, glm::radians(rotationY), 0.0f));
						engine->AddEntity(newEntity);
					}
					numModelsToPlace--;
				}
			}
		}
		if (!filled) break;
	}
}

void EntitiesPatch::ReadFrom(core::utils::IDeserializer* source)
{
	BaseGameEntity::ReadFrom(source);
	source->ReadParameter("min_height", &mHeightMin);
	source->ReadParameter("max_height", &mHeightMax);
	source->ReadParameter("wide", &mWide);
	source->ReadParameter("lenght", &mLength);
	source->ReadParameter("density", &mDensity);
	source->ReadParameter("allow_intersection", &mIsIntersectionAllowed);

	ReadModels(source);
}

void EntitiesPatch::ReadModels(core::utils::IDeserializer* source)
{
	if (source->HasAttribute("models"))
	{
		source->BeginAttribute(std::string("models"));
		unsigned int numElements = source->ReadNumberOfElements();

		source->BeginAttribute();
		do
		{
			std::string modelName;
			int percentage;
			source->ReadParameter("name", modelName);
			source->ReadParameter("percentage", &percentage);
			mModelsName.push_back(std::pair<std::string, int>(modelName, percentage));
			source->NextAttribute();
			numElements--;

		} while (numElements > 0);

		source->EndAttribute();
		source->EndAttribute();
	}
}

