#include "stdafx.h"
#include "Run.h"
#include "src/resources/entities/Player/Player.h"
#include "src/resources/components/CharacterComponent.h"
#include "src/resources/events/characterControllerEvents/TurnEvent.h"
#include "src/resources/events/characterControllerEvents/ForwardEvent.h"
#include "src/resources/events/characterControllerEvents/BackwardEvent.h"

void Run::OnInit()
{
	mCharacterComponent = GetContext()->GetCharacterComponent();
	mPlayer = GetContext()->GetPlayer();
}

void Run::OnEnter(float deltaTime)
{
	mPlayer->PlayAnimation("animation_0");
}

void Run::OnUpdate(float deltaTime)
{
	UpdateEvents(deltaTime);
}

void Run::UpdateEvents(float deltaTime)
{
	while (mCharacterComponent->HasEvents())
	{
		if (mCharacterComponent->IsNextEventOfType<TurnEvent>())
		{
			mPlayer->TreatTurnEvent(deltaTime, mCharacterComponent->ConsumeEvent());
		}
		else if (mCharacterComponent->IsNextEventOfType<ForwardEvent>() || mCharacterComponent->IsNextEventOfType<BackwardEvent>())
		{
			mPlayer->TreatMoveEvent(deltaTime, mCharacterComponent->ConsumeEvent());
		}
		else 
		{
			break;
		}
	}
}

void Run::TreatForwardBackwardEvent(float deltaTime)
{
	if (mCharacterComponent->IsNextEventOfType<ForwardEvent>() || mCharacterComponent->IsNextEventOfType<BackwardEvent>())
	{
		mPlayer->TreatMoveEvent(deltaTime, mCharacterComponent->ConsumeEvent());
	}
}
