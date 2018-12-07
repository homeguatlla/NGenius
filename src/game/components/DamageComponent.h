#pragma once
#include "../../resources/components/IComponent.h"

class DamageComponent :	public IComponent
{
	float mDamage;

public:
	
	DamageComponent(float damage);
	virtual ~DamageComponent();

	float GetDamage() const;
	void SetDamage(float damage);

private:
	// Heredado v�a IComponent
	IComponent * DoClone() const override;
};

