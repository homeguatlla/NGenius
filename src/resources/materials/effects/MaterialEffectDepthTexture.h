#pragma once
#include "../IMaterialEffect.h"
#include <string>

class ITexture;

class MaterialEffectDepthTexture : public IMaterialEffect
{
	ITexture* mTexture;
	float mTile;
	std::string mTextureName;

public:
	MaterialEffectDepthTexture();
	MaterialEffectDepthTexture(ITexture* texture, float tile);
	virtual ~MaterialEffectDepthTexture();

	ITexture* GetDepthTexture() const;
	float GetTile() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectDepthTexture* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectDepthTexture"); }
	static IMaterialEffect* Create(IMaterial* material);
	void Build(TexturesLibrary* texturesLibrary) override;

	// Heredado v�a IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

