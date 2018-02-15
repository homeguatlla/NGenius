#pragma once
#include "IRenderer.h"


class ModelGeometry;
class IShaderProgram;
class Texture;
class ICamera;
class Light;

class ModelRenderer : public IRenderer
{
	std::vector<glm::vec2> mTextureCoords;
	std::vector<glm::vec3> mNormals;
	
protected:
	ModelGeometry* mModel;
	Texture* mTexture;
	const Texture* mTextureShadowmap;
	const Light* mLight;

	unsigned int mMatrixVBO;
	glm::mat4 mShadowSpaceMatrix;
	int mPFCCounter;

private:
	unsigned int mTextureCoordsVBO;
	unsigned int mNormalVBO;
	

public:
	ModelRenderer(ModelGeometry* model, IShaderProgram* shader, Texture* texture, const Light* light);
	~ModelRenderer();

	ModelRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	bool HasFog() const override;
	void EnableFog(bool enable) override;
	bool HasClippingPlane() const override;

	void SetShadowMapParameters(const Texture* shadowMap, const glm::mat4& matrix, int pfcCounter);

	bool IsCastingShadows() const override;

private:
	void Draw() override;
	bool IsInstancingAllowed() const override;

protected:
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;
};

