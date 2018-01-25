#pragma once
#include "IRenderer.h"

class IShaderProgram;

class ShadowRenderer : public IRenderer
{
	unsigned int mMVPMatrixID;

public:
	ShadowRenderer(IShaderProgram* shader);
	~ShadowRenderer();

	ShadowRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexsBuffersManager) override;
	bool IsInstancingAllowed() const override;
	bool HasFog() const override;
	bool HasClippingPlane() const override;

private:
	void Create();
	void PreRender(VertexBuffersManager& vertexsBuffersManager) override;
	void Draw() override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;
};

