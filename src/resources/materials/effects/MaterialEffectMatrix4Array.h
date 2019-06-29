#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>
#include <vector>

class MaterialEffectMatrix4Array : public IMaterialEffect
{
	std::vector<glm::mat4> mValues;

public:
	MaterialEffectMatrix4Array();
	MaterialEffectMatrix4Array(const std::vector<glm::mat4>& values);
	virtual ~MaterialEffectMatrix4Array();

	void SetValues(const std::vector<glm::mat4>& values);
	std::vector<glm::mat4>& GetValues();

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectMatrix4Array* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectMatrix4Array"); }
	static IMaterialEffect* Create(IMaterial* material);

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

