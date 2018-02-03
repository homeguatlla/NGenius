#pragma once
#include "../GameEntity.h"
#include <string>

class Transformation;
class IShaderProgram;
class FontType;

class Text : public GameEntity
{
	static int IDCounter;

	int mTextID;
	std::string mText;
	unsigned int mWidth;
	unsigned int mHeight;
	bool mIsCentered;
	glm::vec4 mColor;

public:
	Text(Transformation* transformation, IShaderProgram* shader, FontType* font, const std::string& text, bool isText3D, const glm::vec4& color, unsigned int width, unsigned int height, bool isCentered);
	~Text();

	unsigned int GetID() const;
	void UpdateText(const std::string& text);
	void SetColor(const glm::vec4& color);
	void SetOutlineColor(const glm::vec4& color);
	void SetBorderParameters(float width, float edge, float borderWidth, float borderEdge);
	void SetShadow(const glm::vec2& offset);
};

