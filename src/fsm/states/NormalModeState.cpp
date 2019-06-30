#include "stdafx.h"
#include "NormalModeState.h"
#include "../../NGenius.h"
#include "../../resources/camera/ICamera.h"

#include <GLFW\glfw3.h>


NormalModeState::NormalModeState() : mIsFreeModeKeyPressed(false)
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
	mIsFreeModeKeyPressed = false;
	mEngine->RegisterAllEventsInputListener(this);
	mEngine->ChangeToCamera(mEngine->GetFreeCamera()->GetName(), mEngine->GetGameplayCamera()->GetName());
}

void NormalModeState::OnExit(float deltaTime)
{
	mEngine->UnRegisterInputListener(this);
	mIsFreeModeKeyPressed = false;
}

void NormalModeState::OnUpdate(float deltaTime)
{
}

void NormalModeState::OnKey(int key, int action)
{
	mIsFreeModeKeyPressed = (key == GLFW_KEY_F10 && action == GLFW_RELEASE);
}