#pragma once
#include "ModelRenderer.h"

class ModelNormalMapRenderer :	public ModelRenderer
{
	std::vector<glm::vec3> mTangents;

	Texture* mNormalmapTexture;
	unsigned int mTangentVBO;

public:
	ModelNormalMapRenderer(ModelGeometry* model, IShaderProgram* shader, Texture* texture, Texture* normalmap, const Light* light);
	~ModelNormalMapRenderer();

	const std::string GetName() const override;

private:
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager);
};

