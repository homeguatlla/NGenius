#include "stdafx.h"
#include "Walk.h"
#include "src/resources/entities/Player/Player.h"

void Walk::OnInit()
{
	mPlayer = GetContext()->GetPlayer();
}

void Walk::OnEnter(float deltaTime)
{
	mPlayer->PlayAnimation("animation_0");
	mPlayer->EnableRun(false);
}

void Walk::OnUpdate(float deltaTime)
{
	mPlayer->PerformMovement(deltaTime);
}

