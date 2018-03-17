#include "stdafx.h"
#include "JumpEvent.h"


JumpEvent::JumpEvent()
{
}


JumpEvent::~JumpEvent()
{
}

JumpEvent* JumpEvent::DoClone(const void* data) const
{
	return new JumpEvent();
}
