#include "stdafx.h"
#include "Text.h"
#include "../Transformation.h"
#include "../../renderer/IRenderer.h"
#include "../../renderer/TextRenderer.h"
#include "../../renderer/GUITextRenderer.h"

int Text::IDCounter = 0;

Text::Text(Transformation* transformation, IShaderProgram* shader, FontType* font, const std::string& text, bool isText3D, const glm::vec4& color, unsigned int width, unsigned int height, bool isCentered) :
	GameEntity(transformation),
	mWidth(width),
	mHeight(height),
	mIsCentered(isCentered),
	mColor(color)
{
	mTextID = ++IDCounter;
	assert(shader != nullptr);
	assert(font != nullptr);
	
	isText3D ? SetRenderer(new TextRenderer(shader, font, mColor, mTextID)) : SetRenderer(new GUITextRenderer(shader, font, mColor, mTextID));
	UpdateText(text);
}


Text::~Text()
{
}

unsigned int Text::GetID() const
{
	return mTextID;
}

void Text::SetColor(const glm::vec4& color)
{
	static_cast<TextRenderer*>(GetRenderer())->SetColor(color);
}

void Text::SetOutlineColor(const glm::vec4& color)
{
	static_cast<TextRenderer*>(GetRenderer())->SetOutlineColor(color);
}

void Text::SetBorderParameters(float width, float edge, float borderWidth, float borderEdge)
{
	static_cast<TextRenderer*>(GetRenderer())->SetBorderParameters(width, edge, borderWidth, borderEdge);
}

void Text::SetShadow(const glm::vec2& offset)
{
	static_cast<TextRenderer*>(GetRenderer())->SetShadow(offset);
}

void Text::UpdateText(const std::string& text)
{
	mText = text;
	TextRenderer* renderer = static_cast<TextRenderer*>(GetRenderer());
	renderer->Create(mText, mWidth, mHeight, mIsCentered);
}