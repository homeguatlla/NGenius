#include "stdafx.h"
#include "Jump.h"
#include "src/resources/entities/Player/Player.h"

void Jump::OnInit()
{
	mPlayer = GetContext()->GetPlayer();
}

void Jump::OnEnter(float deltaTime)
{
	mPlayer->StopAnimations();
	mPlayer->PerformJump(deltaTime);
}

