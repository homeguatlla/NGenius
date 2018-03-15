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

GameEvent* GameEvent::Clone() const
{
	GameEvent* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	return clone;
}