#pragma once
#include "IRenderer.h"

class Texture;
class IShaderProgram;

class QuadRenderer : public IRenderer
{
protected:
	std::vector<glm::vec2> mTextureCoords;
	const Texture* mTexture;
	float mWidth;
	float mHeight;
	bool mIsBillboard;
	unsigned int mTextureCoordsVBO;
	unsigned int mMatrixVBO;

public:
	explicit QuadRenderer(IShaderProgram* shader, const Texture* texture, float width, float height);
	~QuadRenderer();

	QuadRenderer* DoClone() const override;
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	bool IsInstancingAllowed() const override;
	bool HasFog() const override;
	bool HasClippingPlane() const override;
	void SetBillboard(bool isBillboard);

private:
	void Create();
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void Draw() override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;

protected:
	//used for billboarding the quad. Modifies the model matrix as a result
	void ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix);
};

