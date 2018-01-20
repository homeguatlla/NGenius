#include "stdafx.h"
#include "IComponent.h"
#include "../GameEntity.h"

IComponent::IComponent()
{
}


IComponent::~IComponent()
{
}

void IComponent::SetParent(GameEntity* parent)
{
	mParent = parent;
}

IComponent* IComponent::Clone() const
{
	IComponent* clone = DoClone();
	assert(typeid(*clone) == typeid(*this));

	return clone;
}