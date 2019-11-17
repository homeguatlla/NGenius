#include "stdafx.h"
#include "GameEvent.h"

#include <typeinfo>
#include <assert.h>

GameEvent::GameEvent()
{
}


GameEvent::~GameEvent()
{
}

std::shared_ptr<GameEvent> GameEvent::Clone(const void* data) const
{
	std::shared_ptr<GameEvent> clone = DoClone(data);

	assert(typeid(*clone) == typeid(*this));

	return clone;
}