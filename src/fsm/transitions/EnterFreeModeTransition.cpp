#include "stdafx.h"
#include "EnterFreeModeTransition.h"
#include "../../NGenius.h"

#include <GLFW\glfw3.h>
#include <functional>

EnterFreeModeTransition::EnterFreeModeTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination),
	mIsFreeModeActive(false)
{
	mEngine = GetContext()->GetEngine();
	mEngine->RegisterAllEventsInputListener(this);
}

EnterFreeModeTransition::~EnterFreeModeTransition()
{
	mEngine->UnRegisterInputListener(this);
}

void EnterFreeModeTransition::OnInit()
{

}

void EnterFreeModeTransition::OnKey(int key, int action)
{
	mIsFreeModeActive = (key == GLFW_KEY_F10 && action == GLFW_PRESS);
}

void EnterFreeModeTransition::OnMouseScroll(int button, float scroll)
{
}

void EnterFreeModeTransition::OnMouseButton(int button, int action, int mods)
{
}

void EnterFreeModeTransition::OnMouseCursorPos(double x, double y)
{
}

bool EnterFreeModeTransition::CanPerformTransition() const
{
	return mIsFreeModeActive;
}