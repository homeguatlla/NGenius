#include "stdafx.h"
#include "NormalModeState.h"
#include "../../NGenius.h"
#include "../../resources/camera/ICamera.h"
#include "../../resources/entities/Text.h"


#include <GLFW\glfw3.h>


NormalModeState::NormalModeState()
{

}

NormalModeState::~NormalModeState()
{
	mEngine->UnRegisterInputListener(this);
}

void NormalModeState::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

void NormalModeState::OnEnter(float deltaTime)
{
	GetContext()->GetFreeModeText()->GetRenderer()->SetVisibility(false);
	GetContext()->ResetFreeMode();
	mEngine->RegisterAllEventsInputListener(this);
	if (mEngine->GetGameplayCamera() != nullptr)
	{
		mEngine->ChangeToCamera(mEngine->GetFreeCamera()->GetName(), mEngine->GetGameplayCamera()->GetName());
	}
	mEngine->SetDebugModeEnabled(false);
}

void NormalModeState::OnExit(float deltaTime)
{
	mEngine->UnRegisterInputListener(this);
	GetContext()->GetFreeModeText()->GetRenderer()->SetVisibility(true);
	mEngine->SetDebugModeEnabled(true);
}

void NormalModeState::OnUpdate(float deltaTime)
{
}

void NormalModeState::OnKey(int key, int action)
{
	GetContext()->SetEnableFreeModeCamera(key == GLFW_KEY_F10 && action == GLFW_RELEASE);
	GetContext()->SetEnableFreeModePlayer(key == GLFW_KEY_F11 && action == GLFW_RELEASE);
}