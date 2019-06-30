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

void EnterNormalModeTransition::OnInit()
{
	mNGenius = GetContext()->GetNGenius();
	mNGenius->RegisterInputHandler(std::bind(&EnterNormalModeTransition::UpdateInput, this, std::placeholders::_1));
}

bool EnterNormalModeTransition::CanPerformTransition() const
{
	return mIsNormalModeActive;
}

void EnterNormalModeTransition::UpdateInput(GLFWwindow* window)
{
	mIsNormalModeActive = glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS;
}