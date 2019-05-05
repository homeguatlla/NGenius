#pragma once
#include "../IMaterialEffect.h"
#include <string>

class ITexture;

class MaterialEffectNormalTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;
	std::string mTextureName;

public:
	MaterialEffectNormalTexture();
	MaterialEffectNormalTexture(ITexture* texture, float tile);
	virtual ~MaterialEffectNormalTexture();

	ITexture* GetNormalTexture() const;
	float GetTile() const;

	void CopyValuesFrom(IMaterial* material) override;

	void Build(TexturesLibrary* texturesLibrary);

	MaterialEffectNormalTexture* DoClone() const override;

	IMaterialEffect * AddNewEffectToMaterial(IMaterial* material) override;

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer * source) override;
	virtual void WriteTo(core::utils::ISerializer * destination) override;
};

