#include "stdafx.h"
#include "NormalModeState.h"
#include "../../NGenius.h"
#include "../../resources/camera/ICamera.h"

void NormalModeState::OnInit()
{
	mEngine = GetContext()->GetEngine();
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