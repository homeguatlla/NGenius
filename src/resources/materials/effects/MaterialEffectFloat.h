#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class MaterialEffectFloat : public IMaterialEffect
{
	float* mValue;

public:
	MaterialEffectFloat(float* value);
	virtual ~MaterialEffectFloat();

	float GetFloat() const;

	MaterialEffectFloat* DoClone() const override;
};

