#include "stdafx.h"
#include "JumpEvent.h"
#include "../Memory.h"


JumpEvent::JumpEvent()
{
}


JumpEvent::~JumpEvent()
{
}

JumpEvent* JumpEvent::DoClone(const void* data) const
{
	return DBG_NEW JumpEvent();
}

GameEvent* JumpEvent::Create()
{
	return DBG_NEW JumpEvent();
}
