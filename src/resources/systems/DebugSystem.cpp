#include "stdafx.h"
#include "DebugSystem.h"
#include "../GameEntity.h"
#include "../../input/InputHandler.h"
#include "../components/DebugComponent.h"
#include "renderSystem/RenderSystem.h"
#include "GLFW/glfw3.h"
#include <algorithm>

DebugSystem::DebugSystem(RenderSystem* renderSystem, InputHandler* inputHandler) :
	mIsBoundingBoxVisible(false),
	mInputHandler(inputHandler),
	mRenderSystem(renderSystem)
{
	mInputHandler->RegisterAllEventsInputListener(this);
}

DebugSystem::~DebugSystem()
{
	mEntities.clear();
	mInputHandler->UnRegisterInputListener(this);
}

void DebugSystem::Update(float elapsedTime)
{
	for (GameEntity* entity : mEntities)
	{
		entity->Update(elapsedTime);
		DebugComponent* debugComponent = entity->GetComponent<DebugComponent>();
		IRenderer* renderer = debugComponent->GetBoundingBoxRenderer();
		mRenderSystem->AddToRender(renderer);
	}
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

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		mRenderSystem->SetOverdrawEnabled(true);
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		mRenderSystem->SetOverdrawEnabled(false);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if(key == GLFW_KEY_G && action == GLFW_PRESS)
	{ 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}