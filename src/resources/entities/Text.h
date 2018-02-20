#pragma once
#include "../GameEntity.h"
#include <string>

class Transformation;
class FontType;
class Model;
class Mesh;
class IMaterial;

class Text : public GameEntity
{
	static int IDCounter;

	int mTextID;
	std::string mText;
	unsigned int mWidth;
	unsigned int mHeight;
	bool mIsCentered;
	glm::vec4 mColor;
	FontType* mFontType;
	Model* mModel;
	Mesh* mMesh;

public:
	Text(	Transformation* transformation, IMaterial* material, FontType* font, const std::string& text, 
			bool isText3D, const glm::vec4& color, unsigned int width, unsigned int height, bool isCentered);
	~Text();

	unsigned int GetID() const;
	void UpdateText(const std::string& text);

private:
	void Create(const std::string& text, unsigned int width, unsigned int height, bool isCentered);
};

