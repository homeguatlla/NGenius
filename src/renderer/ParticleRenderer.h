#pragma once
#include "QuadRenderer.h"

class IShaderProgram;
class Texture;
class ICamera;

class ParticleRenderer : public QuadRenderer
{
	unsigned int mColorVBO;
	const Texture* mDepthTexture;
	float mTile;

public:
	ParticleRenderer(IShaderProgram* shader, const Texture* texture, const Texture* depthTexture, float width, float height);
	~ParticleRenderer();

	ParticleRenderer* DoClone() const override;
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	void SetTile(float tile);

private:
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;
};

