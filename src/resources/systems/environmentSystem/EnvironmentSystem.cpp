#include "stdafx.h"
#include "EnvironmentSystem.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "../../GameEntity.h"
#include "../../components/EnvironmentAffectedComponent.h"
#include "../../components/EnvironmentModificatorComponent.h"
#include "../../materials/IMaterial.h"
#include "../../materials/effects/MaterialEffectFloat.h"
#include "../../materials/effects/MaterialEffectFloat3Array.h"
#include "../../entities/Terrain.h"
#include "../../renderers/IRenderer.h"
#include "WindManager.h"

EnvironmentSystem::EnvironmentSystem() : mTimer(0.0f)
{
	//mWindManager = std::make_unique<WindManager>(4);
}

EnvironmentSystem::~EnvironmentSystem()
{
	mEntities.clear();
	mModificators.clear();
	mModificatorsPositions.clear();
}

unsigned int EnvironmentSystem::GetNumberGameEntities() const
{
	return mEntities.size();
}

void EnvironmentSystem::Update(float deltaTime)
{
	UpdateModificatorsVector();

	mTimer += deltaTime;

	//mWindManager->Update(deltaTime);
	
	for (GameEntity* entity : mEntities)
	{
		if (entity->GetRenderer() != nullptr)
		{
			EnvironmentAffectedComponent* affectedComponent = entity->GetComponent<EnvironmentAffectedComponent>();
			if (affectedComponent->IsAffectedByWind())
			{
				IMaterial* material = entity->GetRenderer()->GetMaterial();
				MaterialEffectFloat* effectTimer = material->GetEffect<MaterialEffectFloat>();
				if (effectTimer != nullptr)
				{
					effectTimer->SetValue(glm::sin(mTimer * 0.05f));
				}
				//TODO mirar los valores de los componentes de environment affected para pasarlos al shader
				//ahora mismo no se está usando la resistencia
				MaterialEffectFloat3Array* effectModificators = material->GetEffect<MaterialEffectFloat3Array>();
				if (effectModificators != nullptr)
				{
					effectModificators->SetValues(mModificatorsPositions);
				}
			}
		}
	}
}

void EnvironmentSystem::UpdateModificatorsVector()
{
	mModificatorsPositions.clear();
	for (GameEntity* entity : mModificators)
	{
		mModificatorsPositions.push_back(entity->GetTransformation()->GetPosition());
	}
}

void EnvironmentSystem::SetTerrain(const Terrain* terrain)
{
	assert(terrain != nullptr);

	mTerrain = terrain;
}

void EnvironmentSystem::AddEntity(GameEntity* entity)
{
	if(entity->HasComponent<EnvironmentAffectedComponent>())
	{
		mEntities.push_back(entity);
	}
	if (entity->HasComponent<EnvironmentModificatorComponent>())
	{
		mModificators.push_back(entity);
	}
}

void EnvironmentSystem::RemoveEntityVector(GameEntity* entity, std::vector<GameEntity*>& vector)
{
	std::vector<GameEntity*>::iterator it = std::find_if(vector.begin(), vector.end(), [&](GameEntity* a) { return a == entity; });
	if (it != vector.end())
	{
		vector.erase(it);
	}
	else
	{
		assert(false);
	}
}

void EnvironmentSystem::RemoveEntity(GameEntity* entity)
{
	if (entity->HasComponent<EnvironmentAffectedComponent>())
	{
		RemoveEntityVector(entity, mEntities);
	}
	if (entity->HasComponent<EnvironmentModificatorComponent>())
	{
		RemoveEntityVector(entity, mModificators);
	}
}

bool EnvironmentSystem::HasEnvironmentComponents(const GameEntity* entity) const
{
	return entity != nullptr && (	entity->HasComponent<EnvironmentAffectedComponent>() || 
									entity->HasComponent<EnvironmentModificatorComponent>());
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

float EnvironmentSystem::GetTimer() const
{
	return mTimer;
}