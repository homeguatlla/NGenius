#include "stdafx.h"
#include "Run.h"
#include "src/resources/entities/Player/Player.h"

void Run::OnInit()
{
	mPlayer = GetContext()->GetPlayer();
}

void Run::OnEnter(float deltaTime)
{
	mPlayer->PlayAnimation("animation_0");
	mPlayer->EnableRun(true);
}

void Run::OnUpdate(float deltaTime)
{
	mPlayer->PerformMovement(deltaTime);
}

void Run::OnExit(float deltaTime)
{

}
