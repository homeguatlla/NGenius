#pragma once
#include "../GameEntity.h"
#include <vector>
#include <string>

class Texture;
class TextureArray;
class IMaterial;
class Model;
class MaterialEffectFloat;
class ITexture;

class Terrain :	public GameEntity
{
	int mNumVertexsSide;
	float mGridSize;
	float mScale;
	bool mIsFlat;
	Texture* mHeightmap;
	Model* mModel;
	MaterialEffectFloat* mMaterialEffectFloat;
	std::vector<glm::vec3> mVertexs;

	std::string mHeightmapName;

public:
	Terrain() = default;
	explicit Terrain(Transformation* transformation);
	explicit Terrain(Transformation* transformation, IMaterial* material, Texture* heightmap, float scale);
	~Terrain();

	Terrain* DoClone() const override { return nullptr; }

	bool IsPointInside(glm::vec2 point) const;
	float GetHeight(glm::vec2 point) const;
	[[deprecated("Replaced by GetHeight, this needs to be reviwed, not working properly.")]]
	float GetHeightFromColor(glm::vec2 point) const;

	void SetFlat(bool isFlat);
	void SetScale(float scale);

	GameEntity* CreateGameEntity()  override;

	void Build(RenderSystem* renderSystem) override;

	// Heredado v�a ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

private:
	float CalculateBarryCenter(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec2& point) const;
	void CalculateY();
	void Create(IMaterial* material, ITexture* heighmap);
};

