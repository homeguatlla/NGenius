#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectClippingPlane : public IMaterialEffect
{
	glm::vec4 mPlane;

public:
	MaterialEffectClippingPlane();
	virtual ~MaterialEffectClippingPlane();

	void SetClippingPlane(const glm::vec4& plane);
	const glm::vec4 GetClippingPlane() const;
	
	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectClippingPlane* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectClippingPlane"); }
	static IMaterialEffect* Create() { return new MaterialEffectClippingPlane(); }

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

