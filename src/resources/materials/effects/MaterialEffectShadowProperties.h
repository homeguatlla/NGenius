#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class ITexture;

class MaterialEffectShadowProperties : public IMaterialEffect
{
	const ITexture* mDepthTexture;
	glm::mat4 mMatrix;
	int mPFCCounter;

public:
	MaterialEffectShadowProperties(int pfcCounter = -1);
	virtual ~MaterialEffectShadowProperties();

	void SetParameters(const ITexture* texture, const glm::mat4& matrix, int pfcCounter);
	const ITexture* GetDepthTexture() const;
	int GetPFCCounter() const;
	const glm::mat4& GetMatrix() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectShadowProperties* DoClone() const override;
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

