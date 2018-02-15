#pragma once
#include "../resources/renderers/IRenderer_.h"
#include <string>

class IShaderProgram;
class FontType;

class TextRenderer : public IRenderer_
{
	FontType* mFontType;
	glm::vec4 mColor;
	glm::vec4 mOutlineColor;
	float mWidth;
	float mEdge;
	float mBorderWidth;
	float mBorderEdge;
	glm::vec2 mShadowOffset;

protected:
	std::vector<glm::vec2> mTextureCoords;
	unsigned int mTextureCoordsVBO;
	int mTextID;

public:
	explicit TextRenderer(IShaderProgram* shader, FontType* font, const glm::vec4& color, const int textID);
	~TextRenderer();

	TextRenderer* DoClone() const override;
	const std::string GetName() const override;
	void Create(const std::string& text, unsigned int width, unsigned int height, bool isCentered);

	bool IsInstancingAllowed() const override;
	//bool HasFog() const override;
	//bool HasClippingPlane() const override;
	//bool IsCastingShadows() const override;

	void SetColor(const glm::vec4& color);
	void SetOutlineColor(const glm::vec4& color);
	void SetBorderParameters(float width, float edge, float borderWidth, float borderEdge);
	void SetShadow(const glm::vec2& offset);

private:
	//void PreRender(VertexBuffersManager& vertexBufferManager) override;
	void Draw() override;
	//void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) override;
	//int GetRenderShaderPassTextureUnit() const override;
};

