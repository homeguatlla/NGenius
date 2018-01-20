#pragma once
#include "IRenderer.h"


class Model;
class IShaderProgram;
class Texture;
class ICamera;
class Light;

class ModelRenderer : public IRenderer
{
	std::vector<glm::vec2> mTextureCoords;
	std::vector<glm::vec3> mNormals;
	
protected:
	Model* mModel;
	const Texture* mTexture;
	const Light* mLight;

	unsigned int mMatrixVBO;

private:
	unsigned int mTextureCoordsVBO;
	unsigned int mNormalVBO;
	

public:
	ModelRenderer(Model* model, IShaderProgram* shader, const Texture* texture, const Light* light);
	~ModelRenderer();

	ModelRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	bool HasFog() const override;
	void EnableFog(bool enable) override;
	bool HasClippingPlane() const override;

private:
	void Draw() override;
	bool IsInstancingAllowed() const override;

protected:
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
};

