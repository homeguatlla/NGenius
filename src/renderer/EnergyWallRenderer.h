#pragma once
#include "IRenderer.h"

class ModelGeometry;
class IShaderProgram;
class Texture;
class ICamera;
class Light;

class EnergyWallRenderer : public IRenderer
{
	std::vector<glm::vec2> mTextureCoords;
	std::vector<glm::vec3> mNormals;

protected:
	ModelGeometry* mModel;
	Texture* mTexture;
	Texture* mDepthTexture;
	
	unsigned int mMatrixVBO;

private:
	unsigned int mTextureCoordsVBO;
	unsigned int mNormalVBO;


public:
	EnergyWallRenderer(ModelGeometry* model, IShaderProgram* shader, Texture* texture, Texture* depthTexture);
	~EnergyWallRenderer();

	EnergyWallRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	bool HasFog() const override;
	void EnableFog(bool enable) override;
	bool HasClippingPlane() const override;
	bool IsCastingShadows() const override;

private:
	void Draw() override;
	bool IsInstancingAllowed() const override;

protected:
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;
};

