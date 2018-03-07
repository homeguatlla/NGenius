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

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFloat* DoClone() const override;
};

