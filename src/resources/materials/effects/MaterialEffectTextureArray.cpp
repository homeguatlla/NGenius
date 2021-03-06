#include "stdafx.h"
#include "MaterialEffectTextureArray.h"
#include "../IMaterial.h"
#include "../../textures/TextureArray.h"
#include "../../textures/TexturesLibrary.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/Log.h"
#include "../Memory.h"

#include <assert.h>

MaterialEffectTextureArray::MaterialEffectTextureArray() :
	mTexture(nullptr)
{
}

MaterialEffectTextureArray::MaterialEffectTextureArray(TextureArray* textureArray) :
mTexture(textureArray)
{
	assert(textureArray != nullptr);
}


MaterialEffectTextureArray::~MaterialEffectTextureArray()
{
}

TextureArray* MaterialEffectTextureArray::GetTextureArray() const
{
	return mTexture;
}

void MaterialEffectTextureArray::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectTextureArray* effect = material->GetEffect<MaterialEffectTextureArray>();
	if (effect != nullptr)
	{
		mTexture = effect->GetTextureArray();
	}
}

MaterialEffectTextureArray* MaterialEffectTextureArray::DoClone() const
{
	return DBG_NEW MaterialEffectTextureArray(*this);
}

IMaterialEffect* MaterialEffectTextureArray::Create(IMaterial* material)
{
	MaterialEffectTextureArray* effect = DBG_NEW MaterialEffectTextureArray();
	material->AddOrReplaceEffect(effect);
	return effect;
}

void MaterialEffectTextureArray::Build(TexturesLibrary* texturesLibrary)
{
	if (!mTextureName.empty())
	{
		mTexture = static_cast<TextureArray*>(texturesLibrary->GetElement(mTextureName));
		if (mTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "TextureArray " << mTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
}

void MaterialEffectTextureArray::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("texture", mTextureName);
}

void MaterialEffectTextureArray::WriteTo(core::utils::ISerializer * destination)
{
}
