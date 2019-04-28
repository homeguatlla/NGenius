#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectFloat2 : public IMaterialEffect
{
	glm::vec2 mValue;

public:
	MaterialEffectFloat2();
	MaterialEffectFloat2(const glm::vec2& value);
	virtual ~MaterialEffectFloat2();

	void SetValue(const glm::vec2& value);
	const glm::vec2 GetValue() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFloat2* DoClone() const override;
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

