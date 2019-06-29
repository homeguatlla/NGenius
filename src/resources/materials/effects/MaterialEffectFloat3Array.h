#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>
#include <vector>

class MaterialEffectFloat3Array : public IMaterialEffect
{
	std::vector<glm::vec3> mValues;

public:
	MaterialEffectFloat3Array();
	MaterialEffectFloat3Array(const std::vector<glm::vec3>& values);
	virtual ~MaterialEffectFloat3Array();

	void SetValues(const std::vector<glm::vec3>& values);
	std::vector<glm::vec3>& GetValues();

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectFloat3Array* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectFloat3Array"); }
	static IMaterialEffect* Create() { return new MaterialEffectFloat3Array(); }

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

