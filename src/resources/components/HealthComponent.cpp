#include "stdafx.h"
#include "HealthComponent.h"


HealthComponent::HealthComponent(float maxLife) : mMaxLife(maxLife), mLife(maxLife)
{
}


HealthComponent::~HealthComponent()
{
}

float HealthComponent::GetLife() const
{
	return mLife;
}

void HealthComponent::SetLife(float life)
{
	mLife = life;
}


IComponent * HealthComponent::DoClone() const
{
	return new HealthComponent(*this);
}