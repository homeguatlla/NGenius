#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class MaterialEffectFloat : public IMaterialEffect
{
	float mValue;

public:
	MaterialEffectFloat();
	MaterialEffectFloat(float value);
	virtual ~MaterialEffectFloat();

	float GetValue() const;
	void SetValue(float value);

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFloat* DoClone() const override;
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

