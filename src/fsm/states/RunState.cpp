#include "stdafx.h"
#include "RunState.h"
#include "../../NGenius.h"
#include "../../resources/camera/ICamera.h"
#include "../../resources/entities/Text.h"


#include <GLFW\glfw3.h>


RunState::RunState()
{

}

RunState::~RunState()
{
	mEngine->UnRegisterInputListener(this);
}

void RunState::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

void RunState::OnEnter(float deltaTime)
{
	GetContext()->GetInformationText()->GetRenderer()->SetVisibility(false);
	GetContext()->ResetFreeMode();
	mEngine->RegisterAllEventsInputListener(this);
	if (mEngine->GetGameplayCamera() != nullptr)
	{
		mEngine->ChangeToCamera(mEngine->GetFreeCamera()->GetName(), mEngine->GetGameplayCamera()->GetName());
	}
	mEngine->SetDebugModeEnabled(true);
}

void RunState::OnExit(float deltaTime)
{
	mEngine->UnRegisterInputListener(this);
	GetContext()->GetInformationText()->GetRenderer()->SetVisibility(true);
	//mEngine->SetDebugModeEnabled(true);
}

void RunState::OnUpdate(float deltaTime)
{
}

void RunState::OnKey(int key, int action)
{
	GetContext()->SetEnableFreeModeCamera(key == GLFW_KEY_F10 && action == GLFW_RELEASE);
	GetContext()->SetEnableFreeModePlayer(key == GLFW_KEY_F11 && action == GLFW_RELEASE);
}