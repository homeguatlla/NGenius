#pragma once
#include "../../resources/components/IComponent.h"

class HealthComponent :	public IComponent
{
	float mLife;
	float mMaxLife;

public:
	
	HealthComponent(float maxLife);
	virtual ~HealthComponent();

	float GetLife() const;
	void SetLife(float life);

private:
	// Heredado v�a IComponent
	IComponent * DoClone() const override;
};

