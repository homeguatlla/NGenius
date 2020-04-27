#include "stdafx.h"
#include "Idle.h"
#include "src/resources/entities/Player/Player.h"
#include "src/resources/components/CharacterComponent.h"
#include "src/resources/events/characterControllerEvents/TurnEvent.h"
#include "src/resources/events/characterControllerEvents/ForwardEvent.h"
#include "src/resources/events/characterControllerEvents/BackwardEvent.h"

void Idle::OnInit()
{
	mCharacterComponent = GetContext()->GetCharacterComponent();
	mPlayer = GetContext()->GetPlayer();
}

void Idle::OnEnter(float deltaTime)
{	
	mPlayer->StopAnimations();
}

void Idle::OnExit(float deltaTime)
{
}

void Idle::OnUpdate(float deltaTime)
{
	UpdateEvents(deltaTime);
}

void Idle::UpdateEvents(float deltaTime)
{
	while (mCharacterComponent->IsNextEventOfType<TurnEvent>())
	{
		auto event = mCharacterComponent->ConsumeEvent();

		if (event->IsOfType<TurnEvent>())
		{
			mPlayer->TreatTurnEvent(deltaTime, event);
		}
	}
}