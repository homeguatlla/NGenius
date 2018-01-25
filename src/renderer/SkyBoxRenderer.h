#pragma once
#include "IRenderer.h"

class IShaderProgram;
class TextureCubemap;

class SkyBoxRenderer :	public IRenderer
{
	const TextureCubemap* mTextureCubemap;
	float mSize;
	glm::vec3 mFogColor;

public:
	SkyBoxRenderer(IShaderProgram* shader, TextureCubemap* cubemap, float size);
	~SkyBoxRenderer();

	SkyBoxRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	bool IsInstancingAllowed() const override;
	void SetFogColor(const glm::vec3& fogColor);
	bool HasFog() const override;
	bool HasClippingPlane() const override;

private:
	void Create();
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;
	void Draw() override;
};

