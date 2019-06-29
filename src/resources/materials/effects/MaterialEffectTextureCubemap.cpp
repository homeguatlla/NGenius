#include "stdafx.h"
#include "MaterialEffectTextureCubemap.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/Log.h"
#include "../../textures/TexturesLibrary.h"
#include "../../textures/TextureCubemap.h"

#include <assert.h>

MaterialEffectTextureCubemap::MaterialEffectTextureCubemap() :
	mTexture1(nullptr),
	mTexture2(nullptr),
	mBlendFactor(0.0f)
{
}

MaterialEffectTextureCubemap::MaterialEffectTextureCubemap(TextureCubemap* textureCubemap1, TextureCubemap* textureCubemap2, float blendFactor) :
mTexture1(textureCubemap1),
mTexture2(textureCubemap2),
mBlendFactor(blendFactor)
{
	assert(textureCubemap1 != nullptr);
	if (textureCubemap2 == nullptr)
	{
		blendFactor = 0.0f;
	}
}

MaterialEffectTextureCubemap::~MaterialEffectTextureCubemap()
{
}

TextureCubemap* MaterialEffectTextureCubemap::GetCubemap1() const
{
	return mTexture1;
}

TextureCubemap* MaterialEffectTextureCubemap::GetCubemap2() const
{
	return mTexture2;
}

float MaterialEffectTextureCubemap::GetBlendFactor() const
{
	return mBlendFactor;
}

void MaterialEffectTextureCubemap::SetBlendFactor(float blendFactor)
{
	mBlendFactor = blendFactor;
}

void MaterialEffectTextureCubemap::SetCubemap1(TextureCubemap* texture)
{
	mTexture1 = texture;
}

void MaterialEffectTextureCubemap::SetCubemap2(TextureCubemap* texture)
{
	mTexture2 = texture;
}

void MaterialEffectTextureCubemap::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectTextureCubemap* effect = material->GetEffect<MaterialEffectTextureCubemap>();
	if (effect != nullptr)
	{
		mTexture1 = effect->GetCubemap1();
		mTexture2 = effect->GetCubemap2();
		mBlendFactor = effect->GetBlendFactor();
	}
}

MaterialEffectTextureCubemap* MaterialEffectTextureCubemap::DoClone() const
{
	return new MaterialEffectTextureCubemap(*this);
}

IMaterialEffect* MaterialEffectTextureCubemap::Create(IMaterial* material)
{
	MaterialEffectTextureCubemap* effect = new MaterialEffectTextureCubemap();
	material->AddEffect(effect);
	return effect;
}

void MaterialEffectTextureCubemap::Build(TexturesLibrary* texturesLibrary)
{
	if (!mTexture1Name.empty())
	{
		mTexture1 = static_cast<TextureCubemap*>(texturesLibrary->GetElement(mTexture1Name));
		if (mTexture1 == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mTexture1Name << " not found. Material Effect couldn't be build \n";
		}
	}
	if (!mTexture2Name.empty())
	{
		mTexture2 = static_cast<TextureCubemap*>(texturesLibrary->GetElement(mTexture2Name));
		if (mTexture2 == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mTexture2Name << " not found. Material Effect couldn't be build \n";
		}
	}
}

void MaterialEffectTextureCubemap::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("texture1", mTexture1Name);
	source->ReadParameter("texture2", mTexture2Name);
	source->ReadParameter("blend_factor", &mBlendFactor);
}

void MaterialEffectTextureCubemap::WriteTo(core::utils::ISerializer * destination)
{
}
