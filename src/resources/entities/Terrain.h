#pragma once
#include "../GameEntity.h"
#include <vector>

class Texture;
class TextureArray;
class IMaterial;
class Model;
class MaterialEffectFloat;

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

public:
	explicit Terrain(Transformation* transformation, IMaterial* material, Texture* heightmap, float scale);
	~Terrain();

	Terrain* DoClone() const override { return nullptr; }

	bool IsPointInside(glm::vec2 point) const;
	float GetHeight(glm::vec2 point) const;
	[[deprecated("Replaced by GetHeight, this needs to be reviwed, not working properly.")]]
	float GetHeightFromColor(glm::vec2 point) const;

	void SetFlat(bool isFlat);
	void SetScale(float scale);

private:
	float CalculateBarryCenter(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec2& point) const;
	void CalculateY();
};

