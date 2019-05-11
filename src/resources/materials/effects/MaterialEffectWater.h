#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>
#include <string>

class ITexture;

class MaterialEffectWater : public IMaterialEffect
{
	ITexture* mReflectionTexture;
	ITexture* mRefractionTexture;
	ITexture* mDistorsionTexture;
	ITexture* mDepthTexture;
	ITexture* mNormalTexture;
	glm::vec4 mColor;
	float mSpeed;	

	std::string mReflectionTextureName;
	std::string mRefractionTextureName;
	std::string mDistorsionTextureName;
	std::string mDepthTextureName;
	std::string mNormalTextureName;

public:
	MaterialEffectWater();
	MaterialEffectWater(ITexture* reflectionTexture, ITexture* refractionTexture, ITexture* distorsionTexture,
		ITexture* normalTexture, ITexture* depthTexture, float waterSpeed, const glm::vec4& waterColor);
	virtual ~MaterialEffectWater();
	
	ITexture* GetReflectionTexture() const;
	ITexture* GetRefractionTexture() const;
	ITexture* GetDepthTexture() const;
	ITexture* GetDistorsionTexture() const;
	ITexture* GetNormalTexture() const;

	const glm::vec4 GetColor() const;
	float GetSpeed() const;
	void SetSpeed(float speed);

	void CopyValuesFrom(IMaterial* material) override;

	MaterialEffectWater* DoClone() const override;
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;
	void Build(TexturesLibrary* texturesLibrary) override;

	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

