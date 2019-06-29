#pragma once
#include "../IMaterialEffect.h"
#include <string>

class TextureArray;

class MaterialEffectTextureArray : public IMaterialEffect
{
	TextureArray* mTexture;
	std::string mTextureName;

public:
	MaterialEffectTextureArray();
	MaterialEffectTextureArray(TextureArray* texture);
	virtual ~MaterialEffectTextureArray();

	TextureArray* GetTextureArray() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectTextureArray* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectTextureArray"); }
	static IMaterialEffect* Create() { return new MaterialEffectTextureArray(); }

	void Build(TexturesLibrary* texturesLibrary) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

