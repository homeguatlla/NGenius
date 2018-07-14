#include "stdafx.h"
#include "EnvironmentSystem.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "../GameEntity.h"
#include "../components/EnvironmentAffectedComponent.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFloat3.h"
#include "../entities/Terrain.h"
#include "../renderers/IRenderer.h"

EnvironmentSystem::EnvironmentSystem()
{
}

EnvironmentSystem::~EnvironmentSystem()
{
	mEntities.clear();
}

unsigned int EnvironmentSystem::GetNumberGameEntities() const
{
	return mEntities.size();
}

void EnvironmentSystem::Update(float deltaTime)
{
	mAccumulatedTime += deltaTime;

	mSpeed = glm::sin(mAccumulatedTime) * 0.5f;

	for (GameEntity* entity : mEntities)
	{
		IMaterial* material = entity->GetRenderer()->GetMaterial();
		MaterialEffectFloat3* effectWind = material->GetEffect<MaterialEffectFloat3>();
		if (effectWind != nullptr)
		{
			effectWind->SetValue(mSpeed * glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}
}

void EnvironmentSystem::SetTerrain(const Terrain* terrain)
{
	assert(terrain != nullptr);

	mTerrain = terrain;
}

void EnvironmentSystem::AddEntity(GameEntity* entity)
{
	if (HasEnvironmentComponents(entity))
	{
		mEntities.push_back(entity);
	}
}

void EnvironmentSystem::RemoveEntity(GameEntity* entity)
{
	if (HasEnvironmentComponents(entity))
	{
		std::vector<GameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](GameEntity* a) { return a == entity; });
		if (it != mEntities.end())
		{
			mEntities.erase(it);
		}
		else
		{
			assert(false);
		}
	}
}

bool EnvironmentSystem::HasEnvironmentComponents(const GameEntity* entity) const
{
	return entity != nullptr && (	entity->HasComponent<EnvironmentAffectedComponent>() );
}

void EnvironmentSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasEnvironmentComponents(entity))
	{
		AddEntity(entity);
	}
}

void EnvironmentSystem::OnGameEntityRemoved(GameEntity* entity)
{
	if (HasEnvironmentComponents(entity))
	{
		RemoveEntity(entity);
	}
}

BaseVisitable<>::ReturnType EnvironmentSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}