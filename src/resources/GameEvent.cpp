#include "stdafx.h"
#include "GameEvent.h"

#include <typeinfo.h>
#include <assert.h>

GameEvent::GameEvent()
{
}


GameEvent::~GameEvent()
{
}

GameEvent* GameEvent::Clone(const void* data) const
{
	GameEvent* clone = DoClone(data);

	assert(typeid(*clone) == typeid(*this));

	return clone;
}