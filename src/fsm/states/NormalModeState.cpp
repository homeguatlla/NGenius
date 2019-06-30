#include "stdafx.h"
#include "NormalModeState.h"
#include "../../NGenius.h"

void NormalModeState::OnInit()
{
	mEngine = GetContext()->GetEngine();
	mGameplayCamera = mEngine->GetCamera("gameplay_camera");
}

void NormalModeState::OnEnter(float deltaTime)
{
	mEngine->ChangeToCamera(std::string("gameplay_render_pass"), mGameplayCamera);
}

void NormalModeState::OnExit(float deltaTime)
{
}

void NormalModeState::OnUpdate(float deltaTime)
{
}