#include "stdafx.h"
#include "IComponent.h"
#include "../GameEntity.h"

IComponent::IComponent() : mIsEnabled(true)
{
}


IComponent::~IComponent()
{
}

void IComponent::SetParent(GameEntity* parent)
{
	mParent = parent;
}

void IComponent::Update(float elapsedTime)
{
	if (IsEnabled())
	{
		UpdateInternal(elapsedTime);
	}
}

bool IComponent::IsEnabled() const
{
	return mIsEnabled;
}

void IComponent::SetEnabled(bool enabled)
{
	mIsEnabled = enabled;
}

IComponent* IComponent::Clone() const
{
	IComponent* clone = DoClone();
	assert(typeid(*clone) == typeid(*this));

	return clone;
}