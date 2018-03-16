#include "stdafx.h"
#include "JumpEvent.h"


JumpEvent::JumpEvent()
{
}


JumpEvent::~JumpEvent()
{
}

JumpEvent* JumpEvent::DoClone() const
{
	return new JumpEvent();
}
