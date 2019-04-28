#pragma once
#include "../IMaterialEffect.h"

class TextureArray;

class MaterialEffectTextureArray : public IMaterialEffect
{
	TextureArray* mTexture;

public:
	MaterialEffectTextureArray();
	MaterialEffectTextureArray(TextureArray* texture);
	virtual ~MaterialEffectTextureArray();

	TextureArray* GetTextureArray() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectTextureArray* DoClone() const override;
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

