#include "stdafx.h"
#include "NextPreviousInventoryItemEvent.h"
#include "../../resources/systems/InputSystem.h"

NextPreviousInventoryItemEvent::NextPreviousInventoryItemEvent() : mIsNext(false)
{
}


NextPreviousInventoryItemEvent::~NextPreviousInventoryItemEvent()
{
}

NextPreviousInventoryItemEvent* NextPreviousInventoryItemEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	NextPreviousInventoryItemEvent* nextPreviousEvent = new NextPreviousInventoryItemEvent();
	nextPreviousEvent->SetIsNext(mouseData->mScroll > 0);

	return nextPreviousEvent;
}

void NextPreviousInventoryItemEvent::SetIsNext(bool isNext)
{
	mIsNext = isNext;
}

bool NextPreviousInventoryItemEvent::IsNext() const
{
	return mIsNext;
}