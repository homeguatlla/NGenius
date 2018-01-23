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
	const Texture* mHeightmap;
	const Texture* mShadowmap;
	const ICamera* mShadowCamera;
	float mScale;
	bool mIsFlat;

public:
	explicit Terrain(Transformation* transformation, IShaderProgram* shader, const Texture* heightmap, const Texture* blendmap, const TextureArray* textureArray, const Texture* shadowmap, const ICamera* shadowCamera, const Light* light, float scale);
	~Terrain();

	Terrain* DoClone() const override { return nullptr; }

	bool IsPointInside(glm::vec2 point) const;
	float GetHeight(glm::vec2 point) const;

	void SetFlat(bool isFlat);
	void SetShadowCamera(const ICamera* camera);

private:
	float CalculateBarryCenter(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec2& point) const;
};

