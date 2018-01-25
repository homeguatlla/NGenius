#pragma once
#include "IRenderer.h"

class IShaderProgram;

class NormalRenderer : public IRenderer
{
	std::vector<glm::vec3> mNormals;

	unsigned int mMatrixID;

	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void Draw() override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;
	bool IsInstancingAllowed() const override;
	bool HasFog() const override;
	bool HasClippingPlane() const override;
	bool IsCastingShadows() const override;

public:
	NormalRenderer(IShaderProgram* shader);
	~NormalRenderer();

	NormalRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void SetNormals(std::vector<glm::vec3>& normals);
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	void Create();
};

