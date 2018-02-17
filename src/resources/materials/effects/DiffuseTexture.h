#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class ITexture;

class DiffuseTexture : public IMaterialEffect
{
	ITexture* mTexture;
	glm::vec3 mColor;
	float mTile;	

public:
	DiffuseTexture(ITexture* texture, const glm::vec3& color, float tile);
	virtual ~DiffuseTexture();

	ITexture* GetDiffuseTexture() const;
	const glm::vec3 GetColor() const;
	float GetTile() const;

	DiffuseTexture* DoClone() const override;
};

