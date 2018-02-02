#pragma once
class ITexture
{
protected:

	unsigned int mTextureID;
	unsigned int mTextureUnit;
	unsigned int mWidth;
	unsigned int mHeight;

public:
	ITexture();
	virtual ~ITexture();

	unsigned int GetID() const;
	unsigned int GetUnit() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	void SetActive(bool active);
};

