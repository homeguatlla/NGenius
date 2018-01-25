#include "stdafx.h"
#include "TextRenderer.h"

#include "../resources/shaders/IShaderProgram.h"
#include "../resources/shaders/TextShader.h"
#include "../resources/font/FontType.h"
#include "../resources/textures/ITexture.h"
#include "../resources/camera/ICamera.h"
#include "../resources/Transformation.h"
#include "../resources/GameEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

const float EXTRA_CHARACTER_PADDING = 0.0f;

TextRenderer::TextRenderer(IShaderProgram* shader, FontType* font, const glm::vec4& color, const int textID) : 
IRenderer(shader),
mFontType(font),
mTextID(textID),
mColor(color),
mOutlineColor(0.0f),
mWidth(0.5f),
mEdge(0.1f),
mBorderWidth(0.0f),
mBorderEdge(0.0f),
mShadowOffset(0.0f)
{
	assert(font != nullptr);
	assert(font->GetTexture() != nullptr);
	SetLayer(IRenderer::LAYER_PARTICLES);
	SetTransparency(true);
	mBitRenderInformation.SetTexture(font->GetTexture()->GetID());
}


TextRenderer::~TextRenderer()
{
	if (mIsPrerendered)
	{
		glDeleteBuffers(1, &mTextureCoordsVBO);
	}
}


TextRenderer* TextRenderer::DoClone() const
{
	TextRenderer* clone = new TextRenderer(*this);

	return clone;
}

const std::string TextRenderer::GetName() const
{
	return "TextRenderer_" + std::to_string(mTextID);
}

void TextRenderer::SetColor(const glm::vec4& color)
{
	mColor = color;
}

void TextRenderer::SetOutlineColor(const glm::vec4& color)
{
	mOutlineColor = color;
}

void TextRenderer::SetBorderParameters(float width, float edge, float borderWidth, float borderEdge)
{
	mWidth = width;
	mEdge = edge;
	mBorderWidth = borderWidth;
	mBorderEdge = borderEdge;
}

void TextRenderer::SetShadow(const glm::vec2& offset)
{
	mShadowOffset = offset;
}

void TextRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	glDisable(GL_CULL_FACE);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!mIsPrerendered && vertexBufferManager.HasVAO(GetName()))
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexs.size() * sizeof(glm::vec3), &mVertexs[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(unsigned int), &mIndexes[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, mTextureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, mTextureCoords.size() * sizeof(glm::vec2), &mTextureCoords[0], GL_STATIC_DRAW);
	}
	IRenderer::Render(camera, vertexBufferManager);

	glDisable(GL_BLEND);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
}

bool TextRenderer::IsInstancingAllowed() const
{
	return false;
}

bool TextRenderer::HasFog() const
{
	return false;
}

bool TextRenderer::HasClippingPlane() const
{
	return false;
}

bool TextRenderer::IsCastingShadows() const
{
	return false;
}

void TextRenderer::Create(const std::string& text, unsigned int width, unsigned int height, bool isCentered)
{
	std::vector<glm::vec3> vertexs;
	std::vector<unsigned int> indexs;

	mTextureCoords.clear();
	glm::vec3 position(0.0f);

	float baseLine = 0.0f;// mFontType->mCommon.lineHeight - mFontType->mCommon.base;
	for (unsigned int i = 0; i < text.size(); ++i)
	{
		char character = text[i];
		const FontType::Char* characterType = mFontType->GetCharacter(static_cast<unsigned int>(character));
		if (characterType != nullptr)
		{
			vertexs.push_back(position + glm::vec3(0.0f, characterType->height, 0.0f) + glm::vec3(characterType->xOffest - EXTRA_CHARACTER_PADDING, -characterType->height - characterType->yOffset + EXTRA_CHARACTER_PADDING, 0.0f));
			vertexs.push_back(position + glm::vec3(characterType->width, characterType->height, 0.0f) + glm::vec3(characterType->xOffest + EXTRA_CHARACTER_PADDING, -characterType->height - characterType->yOffset + EXTRA_CHARACTER_PADDING, 0.0f));
			vertexs.push_back(position + glm::vec3(characterType->width, 0.0f, 0.0f) + glm::vec3(characterType->xOffest + EXTRA_CHARACTER_PADDING, -characterType->height - characterType->yOffset - EXTRA_CHARACTER_PADDING, 0.0f));
			vertexs.push_back(position + glm::vec3(0.0f) + glm::vec3(characterType->xOffest - EXTRA_CHARACTER_PADDING, -characterType->height - characterType->yOffset - EXTRA_CHARACTER_PADDING, 0.0f));

			mTextureCoords.push_back(glm::vec2(	characterType->x / mFontType->mCommon.scaleWidth, 
												1.0f - characterType->y / mFontType->mCommon.scaleHeight));
			mTextureCoords.push_back(glm::vec2(	(characterType->x + characterType->width) / mFontType->mCommon.scaleWidth, 
												1.0f - characterType->y / mFontType->mCommon.scaleHeight));
			mTextureCoords.push_back(glm::vec2(	(characterType->x + characterType->width) / mFontType->mCommon.scaleWidth,
												1.0f - (characterType->y + characterType->height) / mFontType->mCommon.scaleHeight));
			mTextureCoords.push_back(glm::vec2(	characterType->x / mFontType->mCommon.scaleWidth, 
												1.0f - (characterType->y + characterType->height) / mFontType->mCommon.scaleHeight));

			position += glm::vec3(characterType->xAdvance, 0.0f, 0.0f);

			int index = i * 4;
			indexs.push_back(index);
			indexs.push_back(index + 1);
			indexs.push_back(index + 2);

			indexs.push_back(index);
			indexs.push_back(index + 2);
			indexs.push_back(index + 3);
		}
	}
	
	SetVertexs(vertexs);
	SetIndexes(indexs);
}

void TextRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	IRenderer::PreRender(vertexBufferManager);

	// 2nd attribute buffer : texture coords
	GLint textureID = mShaderProgram->GetAttributeLocation("textureCoordsModelspace");
	if (textureID != -1)
	{
		mTextureCoordsVBO = vertexBufferManager.CreateVBO("texture_" + GetName());
		glBindBuffer(GL_ARRAY_BUFFER, mTextureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, mTextureCoords.size() * sizeof(glm::vec2), &mTextureCoords[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(textureID);
		glVertexAttribPointer(
			textureID,  // The attribute we want to configure
			2,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		glActiveTexture(GL_TEXTURE0 + mFontType->GetTexture()->GetUnit());
		glBindTexture(GL_TEXTURE_2D, mFontType->GetTexture()->GetID());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void TextRenderer::Draw()
{
	IRenderer::Draw();
}

void TextRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	const glm::mat4 viewMatrix = const_cast<ICamera*>(camera)->GetViewMatrix();
	TextShader* shader = static_cast<TextShader*>(mShaderProgram);
	shader->LoadFontTypeTexture(mFontType->GetTexture()->GetUnit());
	shader->LoadViewMatrix(viewMatrix);
	shader->LoadModelMatrix(mParent->GetTransformation()->GetModelMatrix());
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadColor(mColor);
	shader->LoadOutlineColor(mOutlineColor);
	shader->LoadBorderParameters(mWidth, mEdge, mBorderWidth, mBorderEdge);
	shader->LoadShadow(mShadowOffset);
}

int TextRenderer::GetRenderShaderPassTextureUnit() const
{
	return mFontType->GetTexture()->GetUnit();
}