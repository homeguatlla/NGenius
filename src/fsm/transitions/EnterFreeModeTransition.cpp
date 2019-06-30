#include "stdafx.h"
#include "EnterFreeModeTransition.h"
#include "../../NGenius.h"

#include <GLFW\glfw3.h>
#include <functional>

EnterFreeModeTransition::EnterFreeModeTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination),
	mIsFreeModeActive(false)
{
	mNGenius = GetContext()->GetNGenius();
	mNGenius->RegisterInputHandler(std::bind(&EnterFreeModeTransition::UpdateInput, this, std::placeholders::_1));
}

void EnterFreeModeTransition::OnInit()
{

}

void EnterFreeModeTransition::UpdateInput(GLFWwindow* window)
{
	mIsFreeModeActive = glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS;
}

bool EnterFreeModeTransition::CanPerformTransition() const
{
	return mIsFreeModeActive;
}