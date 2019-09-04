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

bool EntitiesPatch::CanPlaceEntityOnPoint(float radius, glm::vec3& point, std::vector<std::pair<glm::vec3, float>>& pointsWithEntityOver)
{
	if (!mIsIntersectionAllowed)
	{
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
	float radius = 0.0f;

	for (EntitiesListIterator it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		if (it->second.second > radius)
		{
			radius = it->second.second;
		}
	}

	return static_cast<int>(area / (radius * 4.0f)); // a bit more of diameter instead of 2, x 3
}

void EntitiesPatch::FillEntitiesRadiusVector(NGenius* engine)
{
	for (auto& pack : mModelsName)
	{
		float maxRadius = 0.0f;
		std::vector<std::string> names = std::get<0>(pack);
		for (auto model : names)
		{
			IGameEntity* entity = engine->GetGameEntity(model);
			if (entity != nullptr)
			{
				glm::vec3 size = entity->GetRenderer()->GetAABB().GetSize();
				float radius = size.x > size.z ? size.x : size.z;

				mEntities[model] = std::pair<IGameEntity*, float>(entity, radius);
				maxRadius = glm::max(maxRadius, radius);
			}
		}
		
		std::get<1>(pack) = maxRadius;
	}
}

void EntitiesPatch::SpawnEntities(NGenius* engine, int numEntities)
{
	std::vector<std::pair<glm::vec3, float>> pointsWithRadiusAlreadyPlaced;

	for (auto pack : mModelsName)
	{
		int percentage = std::get<2>(pack);
		float radius = std::get<1>(pack);
		int numModelsToPlace = static_cast<int>(percentage * numEntities / 100);
		bool filled = true;
		while (filled && numModelsToPlace > 0)
		{
			glm::vec3 point;
			filled = FillWithRandomPoint(point);
			if (filled)
			{
				bool canPlaceIt = CanPlaceEntityOnPoint(radius, point, pointsWithRadiusAlreadyPlaced);

				if (canPlaceIt)
				{
					std::vector<std::string> names = std::get<0>(pack);
					float rotationY = static_cast<float>(rand() % 360);

					for (auto entityName : names)
					{
						IGameEntity* entity = mEntities[entityName].first;
						BaseGameEntity<GameEntity>* clone = static_cast<BaseGameEntity<GameEntity>*>(entity);
						IGameEntity* newEntity = clone->Clone();
						if (newEntity != nullptr)
						{
							newEntity->GetTransformation()->SetPosition(point);							
							newEntity->GetTransformation()->SetRotation(glm::vec3(0.0f, glm::radians(rotationY), 0.0f));
							engine->AddEntity(newEntity);
						}
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
		unsigned int numPacks = source->ReadNumberOfElements();
		source->BeginAttribute(std::string("pack"));

		do
		{
			unsigned int numModels = source->ReadNumberOfElements();
			int percentage;
			source->ReadParameter("percentage", &percentage);

			source->BeginAttribute();
			std::vector<std::string> pack;
			for (unsigned int i = 0; i < numModels; ++i)
			{
				std::string modelName;
				source->ReadParameter("name", modelName);
				pack.push_back(modelName);

				source->NextAttribute();
			}
			source->EndAttribute();

			mModelsName.push_back(std::make_tuple(pack, std::numeric_limits<float>::min(), percentage));

			source->NextAttribute();
			numPacks--;
		} while (numPacks > 0);

		source->EndAttribute();
	}
}

