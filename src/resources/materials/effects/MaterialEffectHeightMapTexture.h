#pragma once
#include "../IMaterialEffect.h"
#include <string>

class ITexture;

class MaterialEffectHeightMapTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;
	std::string mTextureName;

public:
	MaterialEffectHeightMapTexture();
	MaterialEffectHeightMapTexture(ITexture* texture, float tile);
	virtual ~MaterialEffectHeightMapTexture();

	ITexture* GetHeightMapTexture() const;
	float GetTile() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectHeightMapTexture* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectHeightMapTexture"); }
	static IMaterialEffect* Create() { return new MaterialEffectHeightMapTexture(); }

	void Build(TexturesLibrary* texturesLibrary) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

