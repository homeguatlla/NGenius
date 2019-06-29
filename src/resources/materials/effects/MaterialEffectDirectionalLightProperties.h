#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class MaterialEffectDirectionalLightProperties : public IMaterialEffect
{
	glm::vec3 mSunLightDirection;
	glm::vec3 mSunLightColor;
	

public:
	MaterialEffectDirectionalLightProperties();
	virtual ~MaterialEffectDirectionalLightProperties();

	void SetSunLightProperties(const glm::vec3& sunDirection, const glm::vec3& sunColor);
	const glm::vec3& GetSunLightDirection() const;
	const glm::vec3& GetSunLightColor() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectDirectionalLightProperties* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectDirectionalLightProperties"); }
	static IMaterialEffect* Create() { return new MaterialEffectDirectionalLightProperties(); }

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

