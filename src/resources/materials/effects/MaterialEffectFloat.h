#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class MaterialEffectFloat : public IMaterialEffect
{
	float mValue;

public:
	MaterialEffectFloat(float value);
	virtual ~MaterialEffectFloat();

	float GetValue() const;
	void SetValue(float value);

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFloat* DoClone() const override;
};

