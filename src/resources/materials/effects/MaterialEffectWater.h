#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class ITexture;

class MaterialEffectWater : public IMaterialEffect
{
	ITexture* mReflectionTexture;
	ITexture* mRefractionTexture;
	ITexture* mDistorsionTexture;
	ITexture* mDepthTexture;
	ITexture* mNormalTexture;
	glm::vec4 mColor;
	float mSpeed;	

public:
	MaterialEffectWater(ITexture* reflectionTexture, ITexture* refractionTexture, ITexture* distorsionTexture,
		ITexture* normalTexture, ITexture* depthTexture, float waterSpeed, const glm::vec4& waterColor);
	virtual ~MaterialEffectWater();
	
	ITexture* GetReflectionTexture() const;
	ITexture* GetRefractionTexture() const;
	ITexture* GetDepthTexture() const;
	ITexture* GetDistorsionTexture() const;
	ITexture* GetNormalTexture() const;

	const glm::vec4 GetColor() const;
	float GetSpeed() const;
	void SetSpeed(float speed);

	MaterialEffectWater* DoClone() const override;
};

