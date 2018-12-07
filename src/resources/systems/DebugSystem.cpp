#include "stdafx.h"
#include "DebugSystem.h"
#include "../GameEntity.h"
#include "../../input/InputHandler.h"
#include "../components/DebugComponent.h"
#include "renderSystem/RenderSystem.h"
#include "../../guiTool/GuiTool.h"
#include "GLFW/glfw3.h"
#include <algorithm>

DebugSystem::DebugSystem(RenderSystem* renderSystem, InputHandler* inputHandler) :
	mIsDebugModeEnabled(false),
	mIsBoundingBoxVisible(false),
	mIsOverdrawEnabled(false),
	mIsWireframeEnabled(false),
	mIsPostProcessEnabled(true),
	mInputHandler(inputHandler),
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

bool DebugSystem::HasToBeRegisteredToGameScene() const
{
	return true;
}

bool DebugSystem::HasSpecificComponents(const GameEntity* entity) const
{
	return entity != nullptr && entity->HasComponent<DebugComponent>();
}

BaseVisitable<>::ReturnType DebugSystem::Accept(BaseVisitor & guest)
{
	return BaseVisitable<>::ReturnType();
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
}

void DebugSystem::SetDebugModeEnabled(bool enable)
{
	mIsDebugModeEnabled = enable;
}

bool DebugSystem::IsDebugModeEnabled() const
{
	return mIsDebugModeEnabled;
}

