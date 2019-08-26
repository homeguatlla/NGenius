#include "stdafx.h"
#include "StartState.h"
#include "../../NGenius.h"

#include <GLFW\glfw3.h>


void StartState::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

void StartState::OnEnter(float deltaTime)
{
	mEngine->Start();
}

void StartState::OnExit(float deltaTime)
{
}

void StartState::OnUpdate(float deltaTime)
{
	GetContext()->GoToState(NGeniusState::STATE_RUN);
}
