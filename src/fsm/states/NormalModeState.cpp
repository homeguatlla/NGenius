#include "stdafx.h"
#include "NormalModeState.h"
#include "../../NGenius.h"
#include "../../resources/camera/ICamera.h"

#include <GLFW\glfw3.h>

NormalModeState::NormalModeState() : mIsNormalModeActivated(false)
{

}

NormalModeState::~NormalModeState()
{
	mEngine->UnRegisterInputListener(this);
}

void NormalModeState::OnInit()
{
	mEngine = GetContext()->GetEngine();
	mEngine->RegisterAllEventsInputListener(this);
}

void NormalModeState::OnEnter(float deltaTime)
{
	mEngine->ChangeToCamera(mEngine->GetFreeCamera()->GetName(), mEngine->GetGameplayCamera()->GetName());
}

void NormalModeState::OnExit(float deltaTime)
{
}

void NormalModeState::OnUpdate(float deltaTime)
{
}

bool NormalModeState::IsNormalModeActivated() const
{
	return mIsNormalModeActivated;
}

void NormalModeState::OnKey(int key, int action)
{
	mIsNormalModeActivated = (key == GLFW_KEY_F10 && action == GLFW_PRESS);
}