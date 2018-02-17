#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class ITexture;

class ShadowProperties : public IMaterialEffect
{
	ITexture* mDepthTexture;
	glm::mat4 mMatrix;
	int mPFCCounter;

public:
	ShadowProperties();
	virtual ~ShadowProperties();

	void SetParameters(ITexture* texture, const glm::mat4& matrix, int pfcCounter);
	ITexture* GetDepthTexture() const;
	int GetPFCCounter() const;
	const glm::mat4& GetMatrix() const;

	ShadowProperties* DoClone() const override;
};

