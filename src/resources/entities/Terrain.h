#pragma once
#include "src/resources/BaseGameEntity.h"
#include <vector>
#include <string>

class Texture;
class TextureArray;
class IMaterial;
class Model;
class MaterialEffectFloat;
class ITexture;

class Terrain :	public BaseGameEntity<Terrain>
{
	int mNumVertexsSide;
	float mGridSize;
	float mScale;
	Texture* mHeightmap;
	Model* mModel;
	MaterialEffectFloat* mMaterialEffectFloat;
	std::vector<glm::vec3> mVertexs;

	std::string mHeightmapName;
	std::string mModelName;

public:
	Terrain() = default;
	explicit Terrain(Transformation* transformation);
	explicit Terrain(Transformation* transformation, IMaterial* material, Texture* heightmap, float scale);
	~Terrain();

	std::shared_ptr<IGameEntity> DoClone() const override { return nullptr; }

	bool IsPointInside(glm::vec2 point) const;
	float GetHeight(glm::vec2 point) const;
	[[deprecated("Replaced by GetHeight, this needs to be reviwed, not working properly.")]]
	float GetHeightFromColor(glm::vec2 point) const;

	void SetScale(float scale);

	void Build(NGenius* engine) override;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

	static std::string GetClassName() { return std::string("terrain"); }
	static std::shared_ptr<IGameEntity> DoCreate();

private:
	
	float CalculateBarryCenter(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec2& point) const;
	void CalculateY();
	void CreateTerrain(IMaterial* material, ITexture* heighmap);

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

