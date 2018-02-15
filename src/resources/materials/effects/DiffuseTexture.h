#pragma once
#include "../IMaterialEffect.h"

class ITexture;

class DiffuseTexture : public IMaterialEffect
{
	ITexture* mTexture;

public:
	DiffuseTexture(ITexture* texture);
	virtual ~DiffuseTexture();

	ITexture* GetDiffuseTexture() const;
	
	DiffuseTexture* DoClone() const override;
};

