#pragma once
#include "../IMaterialEffect.h"
#include "glm/glm.hpp"

#include <string>

class ITexture;

class MaterialEffectParticle : public IMaterialEffect
{
	ITexture* mTexture;
	ITexture* mDepthTexture;
	glm::vec2 mScreenSize;
	float mTile;

	std::string mTextureName;
	std::string mDepthTextureName;

public:
	MaterialEffectParticle();
	MaterialEffectParticle(ITexture* texture, ITexture* depthTexture, const glm::vec2& screenSize, float tile);
	virtual ~MaterialEffectParticle();

	ITexture* GetTexture();
	ITexture* GetDepthTexture();
	const glm::vec2& GetScreenSize() const;
	float GetTile() const;

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectParticle* DoClone() const override;
	static std::string GetClassName() { return std::string("MaterialEffectParticle"); }
	static IMaterialEffect* Create(IMaterial* material);
	void Build(TexturesLibrary* texturesLibrary) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

