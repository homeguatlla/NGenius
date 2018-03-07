#include "stdafx.h"
#include "MaterialEffectParticle.h"
#include "../IMaterial.h"
#include <assert.h>

MaterialEffectParticle::MaterialEffectParticle(ITexture* texture, ITexture* depthTexture, const glm::vec2& screenSize, float tile) :
mTexture(texture),
mDepthTexture(depthTexture),
mScreenSize(screenSize),
mTile(tile)
{
	assert(texture != nullptr);
	assert(depthTexture != nullptr);
}

MaterialEffectParticle::~MaterialEffectParticle()
{
}

ITexture* MaterialEffectParticle::GetTexture()
{
	return mTexture;
}

ITexture* MaterialEffectParticle::GetDepthTexture()
{
	return mDepthTexture;
}

const glm::vec2& MaterialEffectParticle::GetScreenSize() const
{
	return mScreenSize;
}

float MaterialEffectParticle::GetTile() const
{
	return mTile;
}

void MaterialEffectParticle::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectParticle* effect = material->GetEffect<MaterialEffectParticle>();
	if (effect != nullptr)
	{
		mTile = effect->GetTile();
		mScreenSize = effect->GetScreenSize();
		mTexture = effect->GetTexture();
		mDepthTexture = effect->GetDepthTexture();
	}
}

MaterialEffectParticle* MaterialEffectParticle::DoClone() const
{
	return new MaterialEffectParticle(*this);
}