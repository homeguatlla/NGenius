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

#include <iostream>

const long HOUR_DAY_SPEED = 300;
const long DAY_PERIOD_SEC = 3600 * 24;

EnvironmentSystem::EnvironmentSystem() :
	mTimer(DAY_PERIOD_SEC * 0.5f / HOUR_DAY_SPEED),
	mSunLight(new SunLight()),
	mDayTime(0)
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
	mTimer = DAY_PERIOD_SEC * 0.5f / HOUR_DAY_SPEED;
}

void EnvironmentSystem::Update(float deltaTime)
{
	mTimer += deltaTime;

	UpdateTime();

	UpdateModificatorsVector();

	for (GameEntity* entity : mEntities)
	{
		if (entity->GetRenderer() != nullptr)
		{
			ApplyWind(entity);
		}
	}
}

void EnvironmentSystem::UpdateTime()
{
	mDayTime = static_cast<long>(mTimer * HOUR_DAY_SPEED);
	mDayTime = mDayTime % DAY_PERIOD_SEC;

	long min = mDayTime / 60;
	long hour = min / 60;
	float dayHour = hour + (min - hour * 60) / 60.0f;
	if (dayHour > 2400)
	{
		dayHour = dayHour - 2400;
	}
	mSunLight->SetDayTime(dayHour);
	//std::cout << dayHour << "\n";
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
		//ahora mismo no se está usando la resistencia
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

void EnvironmentSystem::AddSunLightFrame(float hour, const float rotationAngle, const glm::vec3& color,
	const glm::vec3& fogColor, float fogDensity, float fogGradient, const std::string& cubemapName)
{
	mSunLight->AddFrame(hour, rotationAngle, color, fogColor, fogDensity, fogGradient, cubemapName);
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

float EnvironmentSystem::GetDayTime() const
{
	return mSunLight->GetDayTime();
}

void EnvironmentSystem::SetDayTime(float hour)
{
	mSunLight->SetDayTime(hour);
}

void EnvironmentSystem::SetFogGradient(float gradient)
{
	mSunLight->SetCurrentFrameFogGradient(gradient);
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