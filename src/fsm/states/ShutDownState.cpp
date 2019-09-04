#include "stdafx.h"
#include "ShutDownState.h"
#include "../../NGenius.h"


#include <GLFW\glfw3.h>


void ShutDownState::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

void ShutDownState::OnEnter(float deltaTime)
{
	mEngine->ShutDown();
}

void ShutDownState::OnExit(float deltaTime)
{
}

void ShutDownState::OnUpdate(float deltaTime)
{
}
