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
	MaterialEffectShadowProperties();
	virtual ~MaterialEffectShadowProperties();

	void SetParameters(const ITexture* texture, const glm::mat4& matrix, int pfcCounter);
	const ITexture* GetDepthTexture() const;
	int GetPFCCounter() const;
	const glm::mat4& GetMatrix() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectShadowProperties* DoClone() const override;
};

