#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectFogProperties : public IMaterialEffect
{
	glm::vec3 mColor;
	float mDensity;
	float mGradient;

public:
	MaterialEffectFogProperties();
	virtual ~MaterialEffectFogProperties();

	void SetProperties(const glm::vec3& color, float density, float gradient);

	const glm::vec3 GetColor() const;
	float GetDensity() const;
	float GetGradient() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFogProperties* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectFogProperties"); }
	static IMaterialEffect* Create() { return new MaterialEffectFogProperties(); }

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

