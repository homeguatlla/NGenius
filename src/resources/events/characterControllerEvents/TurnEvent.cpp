#include "stdafx.h"
#include "TurnEvent.h"
#include "../../systems/InputSystem.h"
#include "../Memory.h"

TurnEvent::TurnEvent()
{
}


TurnEvent::~TurnEvent()
{
}

std::shared_ptr<GameEvent> TurnEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	std::shared_ptr<TurnEvent> turnEvent = std::make_shared<TurnEvent>();
	turnEvent->SetTurn(static_cast<float>(mouseData->mCursorX));

	return turnEvent;
}

std::shared_ptr<GameEvent> TurnEvent::Create()
{
	return std::make_shared<TurnEvent>();
}

void TurnEvent::SetTurn(float turn)
{
	mTurn = turn;
}

float TurnEvent::GetTurn() const
{
	return mTurn;
}