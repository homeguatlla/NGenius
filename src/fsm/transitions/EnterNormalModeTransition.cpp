#include "stdafx.h"
#include "EnterNormalModeTransition.h"
#include "../../NGenius.h"

#include <GLFW\glfw3.h>
#include <functional>

EnterNormalModeTransition::EnterNormalModeTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination),
	mIsNormalModeActive(false)
{
		
}

EnterNormalModeTransition::~EnterNormalModeTransition()
{
	mEngine->UnRegisterInputListener(this);
}

void EnterNormalModeTransition::OnInit()
{
	mEngine = GetContext()->GetEngine();
	mEngine->RegisterAllEventsInputListener(this);
}

void EnterNormalModeTransition::OnKey(int key, int action)
{
	mIsNormalModeActive = (key == GLFW_KEY_F10 && action == GLFW_PRESS);
}

bool EnterNormalModeTransition::CanPerformTransition() const
{
	return mIsNormalModeActive;
}