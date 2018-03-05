#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../resources/renderers/IRenderer_.h"

class IShaderProgram;
class Texture;
class TextureArray;
class ICamera;
class Light;

class TerrainRenderer : public IRenderer_
{
	std::vector<glm::vec2> mTextureCoords;

	unsigned int mVertexVAO;
	unsigned int mVertexVBO;
	unsigned int mTextureCoordsVBO;
	unsigned int mIndexVBO;
	GLint mMatrixID;
	Texture* mTextureHeightmap;
	Texture* mTextureBlendmap;
	const Texture* mTextureShadowmap;
	TextureArray* mTextureArray;
	const Light* mLight;
	float mScale;
	glm::vec4 mClippingPlane;
	glm::mat4 mShadowSpaceMatrix;
	int mPFCCounter;

public:
	explicit TerrainRenderer(IShaderProgram* shader, Texture* textureHeightmap, Texture* textureBlendmap, TextureArray* mTextureArray, const Light* light, float scale);
	~TerrainRenderer();

	TerrainRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void SetTextureCoords(const std::vector<glm::vec2>& uv);
	//bool HasClippingPlane() const override;
	//void SetClippingPlane(const glm::vec4& plane) override;
	void SetShadowMapParameters(const Texture* shadowMap, const glm::mat4& matrix, int pfcCounter);
	bool IsInstancingAllowed() const override;
	//bool HasFog() const override;
	//void EnableFog(bool enable) override;
	//bool IsCastingShadows() const override;

private:

	void Draw() override;
	//int GetRenderShaderPassTextureUnit() const override;
};

