#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>
#include <string>

class ITexture;

class MaterialEffectDiffuseTexture : public IMaterialEffect
{
	ITexture* mTexture;
	glm::vec3 mColor;
	float mTile;	
	std::string mTextureName;

public:
	MaterialEffectDiffuseTexture();
	MaterialEffectDiffuseTexture(ITexture* texture, const glm::vec3& color, float tile);
	virtual ~MaterialEffectDiffuseTexture();

	ITexture* GetDiffuseTexture() const;
	const glm::vec3 GetColor() const;
	float GetTile() const;

	void SetDiffuseTexture(ITexture* texture);
	void SetTile(float tile);

	void CopyValuesFrom(IMaterial* material) override;
	IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) override;
	void Build(TexturesLibrary* texturesLibrary) override;

	MaterialEffectDiffuseTexture* DoClone() const override;


	// Heredado vía IMaterialEffect
	void ReadFrom(core::utils::IDeserializer * source) override;
	void WriteTo(core::utils::ISerializer * destination) override;
};

