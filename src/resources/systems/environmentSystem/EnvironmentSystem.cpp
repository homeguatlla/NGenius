#include "stdafx.h"
#include "EnvironmentSystem.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp> 

#include "SunLight.h"

#include "../../GameEntity.h"
#include "../../components/EnvironmentAffectedComponent.h"
#include "../../components/EnvironmentModificatorComponent.h"
#include "../../materials/IMaterial.h"
#include "../../materials/effects/MaterialEffectFloat.h"
#include "../../materials/effects/MaterialEffectFloat3Array.h"
#include "../../renderers/IRenderer.h"
#include "../../entities/Terrain.h"



EnvironmentSystem::EnvironmentSystem() : 
	mTimer(0.0f),
	mSunLight(new SunLight())
{
}

EnvironmentSystem::~EnvironmentSystem()
{
	mEntities.clear();
	mModificators.clear();
	mModificatorsPositions.clear();
	delete mSunLight;
}

unsigned int EnvironmentSystem::GetNumberGameEntities() const
{
	return mEntities.size();
}

void EnvironmentSystem::Start()
{
	mSunLight->AddFrame(1200.0f, 90.0f, glm::vec3(0.99f, 0.96f, 0.80f), glm::vec3(89.0f, 120.0f, 143.0f) / 255.0f, 0.004f, 1.5f, "day_cubemap");
	mSunLight->AddFrame(1800.0f, 135.0f, glm::vec3(0.93f, 0.64f, 0.78f), glm::vec3(218.0f, 74.0f, 43.0f) / 255.0f, 0.0f, 1.5f, "day_cubemap");
	mSunLight->AddFrame(2400.0f, 270.0f, glm::vec3(0.86f, 0.64f, 0.93f), glm::vec3(0.0f), 0.0f, 1.5f, "night_cubemap");
	mSunLight->AddFrame(600.0f, 45.0f, glm::vec3(0.9f, 0.81f, 0.65f), glm::vec3(0.9f, 0.81f, 0.65f), 0.02f, 1.5f, "day_cubemap");
}

void EnvironmentSystem::Update(float deltaTime)
{
	UpdateModificatorsVector();

	mTimer += deltaTime;

	for (GameEntity* entity : mEntities)
	{
		if (entity->GetRenderer() != nullptr)
		{
			ApplyWind(entity);
		}
	}
}

void EnvironmentSystem::ApplyWind(GameEntity* entity)
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
		//ahora mismo no se est� usando la resistencia
		MaterialEffectFloat3Array* effectModificators = material->GetEffect<MaterialEffectFloat3Array>();
		if (effectModificators != nullptr)
		{
			effectModificators->SetValues(mModificatorsPositions);
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

glm::vec3 EnvironmentSystem::GetSunLightDirection() const
{
	return mSunLight->GetSunLightDirection();
}

glm::vec3 EnvironmentSystem::GetSunLightColor() const
{
	return mSunLight->GetSunLightColor();
}

void EnvironmentSystem::SetDayHour(float hour)
{
	mSunLight->SetDayHour(hour);
}

float EnvironmentSystem::GetSunLightBlendFactor() const
{
	return mSunLight->GetSunLightBlendFactor();
}

const std::string EnvironmentSystem::GetSkyBoxCubemapOrigin() const
{
	return mSunLight->GetSkyBoxCubemapOrigin();
}

const std::string EnvironmentSystem::GetSkyBoxCubemapDestination() const
{
	return mSunLight->GetSkyBoxCubemapDestination();
}

void EnvironmentSystem::SetSunLightColor(const glm::vec3& color)
{
	mSunLight->SetCurrentFrameColor(color);
}

glm::vec3 EnvironmentSystem::GetFogColor() const
{
	return mSunLight->GetFogColor();
}

float EnvironmentSystem::GetFogDensity() const
{
	return mSunLight->GetFogDensity();
}

float EnvironmentSystem::GetFogGradient() const
{
	return mSunLight->GetFogGradient();
}