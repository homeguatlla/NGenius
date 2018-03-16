#include "stdafx.h"
#include "BackwardEvent.h"


BackwardEvent::BackwardEvent()
{
}


BackwardEvent::~BackwardEvent()
{
}

BackwardEvent* BackwardEvent::DoClone() const
{
	return new BackwardEvent();
}