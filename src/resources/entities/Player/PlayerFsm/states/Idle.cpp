#include "stdafx.h"
#include "Idle.h"
#include "src/resources/entities/Player/Player.h"

void Idle::OnInit()
{
	mPlayer = GetContext()->GetPlayer();
}

void Idle::OnEnter(float deltaTime)
{	
	mPlayer->StopAnimations();
}