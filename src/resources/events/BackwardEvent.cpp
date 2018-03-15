#include "stdafx.h"
#include "BackwardEvent.h"


BackwardEvent::BackwardEvent()
{
}


BackwardEvent::~BackwardEvent()
{
}

GameEvent* BackwardEvent::DoClone() const
{
	return new BackwardEvent();
}