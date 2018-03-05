#include "stdafx.h"
#include "MaterialEffectText.h"

MaterialEffectText::MaterialEffectText(const glm::vec4& color, const glm::vec4& outlineColor, float width, float edge,
	float borderWidth, float borderEdge, const glm::vec2& shadowOffset) :
mColor(color),
mOutlineColor(outlineColor),
mWidth(width),
mEdge(edge),
mBorderWidth(borderWidth),
mBorderEdge(borderEdge),
mShadowOffset(shadowOffset)
{
}

MaterialEffectText::~MaterialEffectText()
{
}

void MaterialEffectText::SetColor(const glm::vec4& color)
{
	mColor = color;
}

glm::vec4 MaterialEffectText::GetColor() const
{
	return mColor;
}

glm::vec4 MaterialEffectText::GetOutlineColor() const
{
	return mOutlineColor;
}

float MaterialEffectText::GetWidth() const
{
	return mWidth;
}

float MaterialEffectText::GetEdge() const
{
	return mEdge;
}

float MaterialEffectText::GetBorderWidth() const
{
	return mBorderWidth;
}

float MaterialEffectText::GetBorderEdge() const
{
	return mBorderEdge;
}

glm::vec2 MaterialEffectText::GetShadowOffset() const
{
	return mShadowOffset;
}

MaterialEffectText* MaterialEffectText::DoClone() const
{
	return new MaterialEffectText(*this);
}