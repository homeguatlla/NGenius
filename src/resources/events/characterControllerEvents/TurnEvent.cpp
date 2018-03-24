#include "stdafx.h"
#include "TurnEvent.h"
#include "../../systems/InputSystem.h"

TurnEvent::TurnEvent()
{
}


TurnEvent::~TurnEvent()
{
}

TurnEvent* TurnEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	TurnEvent* turnEvent = new TurnEvent();
	turnEvent->SetTurn(static_cast<float>(mouseData->mCursorX));

	return turnEvent;
}

void TurnEvent::SetTurn(float turn)
{
	mTurn = turn;
}

float TurnEvent::GetTurn() const
{
	return mTurn;
}