#include "stdafx.h"
#include "InitialState.h"
#include "../../NGenius.h"

#include <GLFW\glfw3.h>

void InitialState::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

void InitialState::OnEnter(float deltaTime)
{
	mEngine->Init(false);
}

void InitialState::OnExit(float deltaTime)
{
	
}

void InitialState::OnUpdate(float deltaTime)
{
	GetContext()->GoToState(NGeniusState::STATE_LOADING);
}
