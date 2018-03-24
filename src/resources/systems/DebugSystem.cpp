#include "stdafx.h"
#include "DebugSystem.h"
#include "../GameEntity.h"
#include "../../input/InputHandler.h"
#include "../components/DebugComponent.h"
#include "GLFW/glfw3.h"
#include <algorithm>

DebugSystem::DebugSystem(InputHandler* inputHandler) :
	mIsBoundingBoxVisible(false)
{
	inputHandler->RegisterAllEventsInputListener(this);
}

DebugSystem::~DebugSystem()
{
	mEntities.clear();
}

void DebugSystem::Update(float elapsedTime)
{
}

bool DebugSystem::HasDebugComponents(GameEntity* entity) const
{
	return entity != nullptr && entity->HasComponent<DebugComponent>();
}

void DebugSystem::AddEntity(GameEntity* entity)
{
	mEntities.push_back(entity);
}

void DebugSystem::RemoveEntity(GameEntity* entity)
{
	if (HasDebugComponents(entity))
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

void DebugSystem::OnKey(int key, int action)
{
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		mIsBoundingBoxVisible = !mIsBoundingBoxVisible;
		for (GameEntity* entity : mEntities)
		{
			DebugComponent* component = entity->GetComponent<DebugComponent>();
			if (component != nullptr)
			{
				component->SetBoundingBoxVisibility(mIsBoundingBoxVisible);
			}
		}
	}
}