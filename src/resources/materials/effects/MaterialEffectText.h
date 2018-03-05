#pragma once
#include "../IMaterialEffect.h"
#include <glm/glm.hpp>

class MaterialEffectText : public IMaterialEffect
{
	glm::vec4 mColor;
	glm::vec4 mOutlineColor;
	float mWidth;
	float mEdge;
	float mBorderWidth;
	float mBorderEdge;
	glm::vec2 mShadowOffset;

public:
	MaterialEffectText(const glm::vec4& color, const glm::vec4& outlineColor, float width, float edge, 
		float borderWidth, float borderEdge, const glm::vec2& shadowOffset);
	virtual ~MaterialEffectText();

	void SetColor(const glm::vec4& color);
	glm::vec4 GetColor() const;
	glm::vec4 GetOutlineColor() const;
	float GetWidth() const;
	float GetEdge() const;
	float GetBorderWidth() const;
	float GetBorderEdge() const;
	glm::vec2 GetShadowOffset() const;

	MaterialEffectText* DoClone() const override;
};

