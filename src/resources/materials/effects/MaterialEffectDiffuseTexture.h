#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class ITexture;

class MaterialEffectDiffuseTexture : public IMaterialEffect
{
	ITexture* mTexture;
	glm::vec3 mColor;
	float mTile;	

public:
	MaterialEffectDiffuseTexture(ITexture* texture, const glm::vec3& color, float tile);
	virtual ~MaterialEffectDiffuseTexture();

	ITexture* GetDiffuseTexture() const;
	const glm::vec3 GetColor() const;
	float GetTile() const;

	MaterialEffectDiffuseTexture* DoClone() const override;
};

