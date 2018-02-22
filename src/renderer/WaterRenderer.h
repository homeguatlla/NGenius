#pragma once
#include "IRenderer.h"


class Texture;
class IShaderProgram;
class Light;

class WaterRenderer : public IRenderer
{
	std::vector<glm::vec2> mTextureCoords;
	Texture* mReflectionTexture;
	Texture* mRefractionTexture;
	Texture* mDistorsionTexture;
	Texture* mNormalTexture;
	Texture* mDepthTexture;
	float mWidth;
	float mLength;
	float mWaterSpeed;
	float mCurrentWaterSpeed;
	glm::vec4 mWaterColor;
	const Light* mLight;

	unsigned int mTextureCoordsVBO;
	unsigned int mMatrixVBO;
public:
	explicit WaterRenderer(IShaderProgram* shader, Texture* reflectionTexture, Texture* refractionTexture, Texture* distorsion, 
		Texture* normal, Texture* depth, float width, float length, float waterSpeed, const glm::vec4& waterColor, const Light* light);
	~WaterRenderer();

	WaterRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	bool IsInstancingAllowed() const override;
	bool HasClippingPlane() const override;
	bool HasFog() const override;
	void EnableFog(bool enable) override;
	bool IsCastingShadows() const override;

private:
	void Create();
	void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void Draw() override;
	void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	int GetRenderShaderPassTextureUnit() const override;
};

