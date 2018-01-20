#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "IRenderer.h"

class IShaderProgram;
class Texture;
class TextureArray;
class ICamera;
class Light;

class TerrainRenderer : public IRenderer
{
	std::vector<glm::vec2> mTextureCoords;

	unsigned int mVertexVAO;
	unsigned int mVertexVBO;
	unsigned int mTextureCoordsVBO;
	unsigned int mIndexVBO;
	unsigned int mMatrixID;
	const Texture* mTextureHeightmap;
	const Texture* mTextureBlendmap;
	const TextureArray* mTextureArray;
	const Light* mLight;
	float mScale;
	glm::vec4 mClippingPlane;

public:
	explicit TerrainRenderer(IShaderProgram* shader, const Texture* textureHeightmap, const Texture* textureBlendmap, const TextureArray* mTextureArray, const Light* light, float scale);
	~TerrainRenderer();

	TerrainRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void SetTextureCoords(const std::vector<glm::vec2>& uv);
	bool HasClippingPlane() const override;
	void SetClippingPlane(const glm::vec4& plane) override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	bool IsInstancingAllowed() const override;
	bool HasFog() const override;
	void EnableFog(bool enable) override;

private:

	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void Draw() override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
};

