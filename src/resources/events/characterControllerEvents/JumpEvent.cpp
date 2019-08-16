#include "stdafx.h"
#include "JumpEvent.h"
#include "../Memory.h"


JumpEvent::JumpEvent()
{
}


JumpEvent::~JumpEvent()
{
}

std::shared_ptr<GameEvent> JumpEvent::DoClone(const void* data) const
{
	return std::make_shared<JumpEvent>();
}

std::shared_ptr<GameEvent> JumpEvent::Create()
{
	return std::make_shared<JumpEvent>();
}
