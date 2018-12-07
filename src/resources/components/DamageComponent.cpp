#include "stdafx.h"
#include "DamageComponent.h"


DamageComponent::DamageComponent(float damage) : mDamage(damage)
{
}


DamageComponent::~DamageComponent()
{
}

float DamageComponent::GetDamage() const
{
	return mDamage;
}

void DamageComponent::SetDamage(float damage)
{
	mDamage = damage;
}


IComponent * DamageComponent::DoClone() const
{
	return new DamageComponent(*this);
}