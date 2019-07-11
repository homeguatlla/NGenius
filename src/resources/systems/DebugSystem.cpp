#include "stdafx.h"
#include "DebugSystem.h"
#include "../../NGenius.h"
#include "../GameEntity.h"
#include "../../input/InputHandler.h"
#include "../components/DebugComponent.h"
#include "renderSystem/RenderSystem.h"
#include "../../guiTool/GuiTool.h"
#include "GLFW/glfw3.h"
#include <algorithm>

DebugSystem::DebugSystem(NGenius* engine, RenderSystem* renderSystem, InputHandler* inputHandler) :
	mIsDebugModeEnabled(false),
	mIsBoundingBoxVisible(false),
	mIsOverdrawEnabled(false),
	mIsWireframeEnabled(false),
	mIsPostProcessEnabled(true),
	mInputHandler(inputHandler),
	mEngine(engine),
	mRenderSystem(renderSystem),
	mIsInitialized(false)
{
}

DebugSystem::~DebugSystem()
{
	mEntities.clear();
	mInputHandler->UnRegisterInputListener(this);
}

void DebugSystem::Start()
{
	mInputHandler->RegisterAllEventsInputListener(this);
	mIsInitialized = true;
}

void DebugSystem::Update(float elapsedTime)
{
	assert(mIsInitialized);

	if (mIsDebugModeEnabled && mIsBoundingBoxVisible)
	{
		for (GameEntity* entity : mEntities)
		{
			entity->Update(elapsedTime);
			DebugComponent* debugComponent = entity->GetComponent<DebugComponent>();
			if(debugComponent->IsEnabled())
			{
				IRenderer* renderer = debugComponent->GetBoundingBoxRenderer();
				mRenderSystem->AddToRender(renderer);
			}
		}
	}
}

bool DebugSystem::HasDebugComponents(const GameEntity* entity) const
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
		mIsOverdrawEnabled = !mIsOverdrawEnabled;
		mRenderSystem->SetOverdrawEnabled(mIsOverdrawEnabled);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		mIsWireframeEnabled = !mIsWireframeEnabled;

		glPolygonMode(GL_FRONT_AND_BACK, mIsWireframeEnabled ? GL_LINE : GL_FILL);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		mIsPostProcessEnabled = !mIsPostProcessEnabled;
		mRenderSystem->SetPostProcessEnabled(mIsPostProcessEnabled);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		GuiTool* guiTool = mRenderSystem->GetGuiTool();
		guiTool->IsVisible() ? guiTool->Hide() : guiTool->Show();
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		mEngine->Reload();
	}
}

void DebugSystem::SetDebugModeEnabled(bool enable)
{
	mIsDebugModeEnabled = enable;
}

bool DebugSystem::IsDebugModeEnabled() const
{
	return mIsDebugModeEnabled;
}

void DebugSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasDebugComponents(entity))
	{
		AddEntity(entity);
	}
}

void DebugSystem::OnGameEntityRemoved(GameEntity* entity)
{
	if (HasDebugComponents(entity))
	{
		RemoveEntity(entity);
	}
}