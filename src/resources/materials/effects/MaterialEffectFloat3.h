#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectFloat3 : public IMaterialEffect
{
	glm::vec3 mValue;

public:
	MaterialEffectFloat3();
	MaterialEffectFloat3(const glm::vec3& value);
	virtual ~MaterialEffectFloat3();

	void SetValue(const glm::vec3& value);
	const glm::vec3 GetValue() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFloat3* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectFloat3"); }
	static IMaterialEffect* Create(IMaterial* material);

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

