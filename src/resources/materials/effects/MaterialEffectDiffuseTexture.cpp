#include "stdafx.h"
#include "MaterialEffectDiffuseTexture.h"
#include "../IMaterial.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/Log.h"
#include "../../textures/TexturesLibrary.h"



MaterialEffectDiffuseTexture::MaterialEffectDiffuseTexture() :
	mTexture(nullptr),
	mColor(glm::vec3(0.0f)),
	mTile(1.0f),
	mTextureName("")
{

}

MaterialEffectDiffuseTexture::MaterialEffectDiffuseTexture(ITexture* texture, const glm::vec3& color, float tile) :
mTexture(texture),
mColor(color),
mTile(tile),
mTextureName("")
{
	assert(texture != nullptr);
}


MaterialEffectDiffuseTexture::~MaterialEffectDiffuseTexture()
{
}

ITexture* MaterialEffectDiffuseTexture::GetDiffuseTexture() const
{
	return mTexture;
}

void MaterialEffectDiffuseTexture::SetDiffuseTexture(ITexture* texture)
{
	assert(texture != nullptr);
	mTexture = texture;
}

const glm::vec3 MaterialEffectDiffuseTexture::GetColor() const
{
	return mColor;
}

float MaterialEffectDiffuseTexture::GetTile() const
{
	return mTile;
}

void MaterialEffectDiffuseTexture::SetTile(float tile)
{
	mTile = tile;
}

void MaterialEffectDiffuseTexture::CopyValuesFrom(IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		mTexture = effectDiffuse->GetDiffuseTexture();
		mTile = effectDiffuse->GetTile();
		mColor = effectDiffuse->GetColor();
	}
}

IMaterialEffect* MaterialEffectDiffuseTexture::Create(IMaterial* material)
{
	MaterialEffectDiffuseTexture* effect = new MaterialEffectDiffuseTexture();
	material->AddEffect(effect);
	return effect;
}

void MaterialEffectDiffuseTexture::Build(TexturesLibrary* texturesLibrary)
{
	if (!mTextureName.empty())
	{
		mTexture = texturesLibrary->GetElement(mTextureName);
		if (mTexture == nullptr)
		{
			Log(Log::LOG_WARNING) << "Texture " << mTextureName << " not found. Material Effect couldn't be build \n";
		}
	}
}

MaterialEffectDiffuseTexture* MaterialEffectDiffuseTexture::DoClone() const
{
	return new MaterialEffectDiffuseTexture(*this);
}

void MaterialEffectDiffuseTexture::ReadFrom(core::utils::IDeserializer * source)
{
	source->ReadParameter("texture", mTextureName);
	source->ReadParameter("color", mColor);
	source->ReadParameter("tile", &mTile);
}

void MaterialEffectDiffuseTexture::WriteTo(core::utils::ISerializer * destination)
{
}
