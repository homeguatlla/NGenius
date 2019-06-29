#include "stdafx.h"
#include "MaterialEffectParticle.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../textures/TexturesLibrary.h"
#include <assert.h>

MaterialEffectParticle::MaterialEffectParticle() :
	mTexture(nullptr),
	mDepthTexture(nullptr),
	mScreenSize(0.0f, 0.0f),
	mTile(1.0f)
{
}

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

IMaterialEffect* MaterialEffectParticle::Create(IMaterial* material)
{
	MaterialEffectParticle* effect = new MaterialEffectParticle();
	material->AddEffect(effect);
	return effect;
}

void MaterialEffectParticle::Build(TexturesLibrary* texturesLibrary)
{
	mTexture = texturesLibrary->GetElement(mTextureName);
	mDepthTexture = texturesLibrary->GetElement(mDepthTextureName);

	assert(mTexture != nullptr);
	assert(mDepthTexture != nullptr);
}

void MaterialEffectParticle::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("tile", &mTile);
	source->ReadParameter("screen_size", mScreenSize);
	source->ReadParameter("depth_texture", mDepthTextureName);
	source->ReadParameter("texture", mTextureName);
}

void MaterialEffectParticle::WriteTo(core::utils::ISerializer * destination)
{
}
