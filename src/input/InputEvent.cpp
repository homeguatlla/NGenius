#include "stdafx.h"
#include "InputEvent.h"

#include <typeinfo.h>
#include <assert.h>

InputEvent::InputEvent()
{
}


InputEvent::~InputEvent()
{
}

InputEvent* InputEvent::Clone() const
{
	InputEvent* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	return clone;
}