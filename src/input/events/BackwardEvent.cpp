#include "stdafx.h"
#include "BackwardEvent.h"


BackwardEvent::BackwardEvent()
{
}


BackwardEvent::~BackwardEvent()
{
}

InputEvent* BackwardEvent::DoClone() const
{
	return new BackwardEvent();
}