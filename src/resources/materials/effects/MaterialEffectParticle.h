#pragma once
#include "../IMaterialEffect.h"
#include "glm/glm.hpp"

class ITexture;

class MaterialEffectParticle : public IMaterialEffect
{
	ITexture* mTexture;
	ITexture* mDepthTexture;
	glm::vec2 mScreenSize;
	float mTile;

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
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;

	// Heredado v�a IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

