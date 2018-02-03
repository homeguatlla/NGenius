#pragma once
#include "../GameEntity.h"


class Texture;
class TextureArray;
class Light;
class IShaderProgram;
class ICamera;

class Terrain :	public GameEntity
{
	int mNumVertexsSide;
	float mGridSize;
	Texture* mHeightmap;
	const Texture* mShadowmap;
	float mScale;
	bool mIsFlat;

public:
	explicit Terrain(Transformation* transformation, IShaderProgram* shader, Texture* heightmap, Texture* blendmap, TextureArray* textureArray, const Texture* shadowmap, const Light* light, float scale);
	~Terrain();

	Terrain* DoClone() const override { return nullptr; }

	bool IsPointInside(glm::vec2 point) const;
	float GetHeight(glm::vec2 point) const;

	void SetFlat(bool isFlat);

private:
	float CalculateBarryCenter(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec2& point) const;
};

