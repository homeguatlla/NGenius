#include "stdafx.h"
#include "OnAir.h"
#include "src/resources/entities/Player/Player.h"

void OnAir::OnInit()
{
	mPlayer = GetContext()->GetPlayer();
}

void OnAir::OnEnter(float deltaTime)
{
	mPlayer->StopAnimations();
}