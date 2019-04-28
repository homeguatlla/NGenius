#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>


class MaterialEffectFloat4 : public IMaterialEffect
{
	glm::vec4 mValue;

public:
	MaterialEffectFloat4();
	MaterialEffectFloat4(const glm::vec4& value);
	virtual ~MaterialEffectFloat4();

	void SetValue(const glm::vec4& value);
	const glm::vec4 GetValue() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFloat4* DoClone() const override;
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

