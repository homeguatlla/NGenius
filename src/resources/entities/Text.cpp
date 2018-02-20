#include "stdafx.h"
#include "Text.h"
#include "../Transformation.h"
#include "../renderers/IRenderer_.h"
#include "../renderers/TextRenderer.h"
#include "../renderers/GUITextRenderer.h"
#include "../models/Model.h"
#include "../models/Mesh.h"
#include "../font/FontType.h"

const float EXTRA_CHARACTER_PADDING = 0.0f;
int Text::IDCounter = 0;

Text::Text(Transformation* transformation, IMaterial* material, FontType* font, 
	const std::string& text, bool isText3D, const glm::vec4& color, unsigned int width, unsigned int height, bool isCentered) :
	GameEntity(transformation),
	mFontType(font),
	mWidth(width),
	mHeight(height),
	mIsCentered(isCentered),
	mColor(color)
{
	mTextID = ++IDCounter;
	assert(material != nullptr);
	assert(font != nullptr);

	mMesh = new Mesh();
	mModel = new Model(mMesh);

	isText3D ? SetRenderer(new TextRenderer(mModel, material)) : SetRenderer(new GUITextRenderer(mModel, material));
	UpdateText(text);
}

Text::~Text()
{
	delete mModel;
}

unsigned int Text::GetID() const
{
	return mTextID;
}

void Text::UpdateText(const std::string& text)
{
	mText = text;
	Create(mText, mWidth, mHeight, mIsCentered);
	if (mModel->IsBuilt())
	{
		mModel->UpdateVBOs();
	}
}

void Text::Create(const std::string& text, unsigned int width, unsigned int height, bool isCentered)
{
	std::vector<glm::vec3> vertexs;
	std::vector<unsigned int> indexs;
	std::vector<glm::vec2> uv;

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

			uv.push_back(glm::vec2(characterType->x / mFontType->mCommon.scaleWidth,
				1.0f - characterType->y / mFontType->mCommon.scaleHeight));
			uv.push_back(glm::vec2((characterType->x + characterType->width) / mFontType->mCommon.scaleWidth,
				1.0f - characterType->y / mFontType->mCommon.scaleHeight));
			uv.push_back(glm::vec2((characterType->x + characterType->width) / mFontType->mCommon.scaleWidth,
				1.0f - (characterType->y + characterType->height) / mFontType->mCommon.scaleHeight));
			uv.push_back(glm::vec2(characterType->x / mFontType->mCommon.scaleWidth,
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

	mMesh->SetVertexs(vertexs);
	mMesh->SetIndexes(indexs);
	mMesh->SetTextureCoords(uv);
}