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
	static std::string GetClassName() { return std::string("MaterialEffectFloat"); }
	static IMaterialEffect* Create(IMaterial* material);

	// Heredado v�a IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

