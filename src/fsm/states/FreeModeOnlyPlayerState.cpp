#include "stdafx.h"
#include "FreeModeOnlyPlayerState.h"

#include "../GameConstants.h"
#include "../../utils/Log.h"
#include "../../resources/entities/Text.h"


#include <GLFW\glfw3.h>
#include <string>

FreeModeOnlyPlayerState::FreeModeOnlyPlayerState()
{
}

FreeModeOnlyPlayerState::~FreeModeOnlyPlayerState()
{
}

void FreeModeOnlyPlayerState::OnEnter(float deltaTime)
{
	FreeModeOnlyCameraState::OnEnter(deltaTime);

	GetContext()->GetFreeModeText()->UpdateText(TEXT_PLAYER_MODE);
	mPlayer->SetEnabled(true);
}

void FreeModeOnlyPlayerState::OnKey(int key, int action)
{
	GetContext()->SetEnableFreeModeCamera(key == GLFW_KEY_F10 && action == GLFW_RELEASE);
	GetContext()->SetEnableFreeModePlayer(key == GLFW_KEY_F11 && action == GLFW_RELEASE);
}

void FreeModeOnlyPlayerState::OnMouseCursorPos(double x, double y)
{
}
