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
	MaterialEffectDiffuseTexture();
	MaterialEffectDiffuseTexture(ITexture* texture, const glm::vec3& color, float tile);
	virtual ~MaterialEffectDiffuseTexture();

	ITexture* GetDiffuseTexture() const;
	const glm::vec3 GetColor() const;
	float GetTile() const;

	void SetDiffuseTexture(ITexture* texture);
	void SetTile(float tile);

	MaterialEffectDiffuseTexture* DoClone() const override;
};

