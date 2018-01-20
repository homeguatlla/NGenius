#pragma once
#include <string>
#include <map>

class ITexture;

class FontType
{
public:
	struct Char
	{
		float x;
		float y;
		float width;
		float height;
		int xOffest;
		int yOffset;
		unsigned int xAdvance;
	};

	struct Info
	{
		std::string face;
		unsigned int size;
	};

	struct Common
	{
		unsigned int lineHeight;
		unsigned int base;
		unsigned int scaleWidth;
		unsigned int scaleHeight;
	};

	
	typedef std::map<unsigned int, Char*>::iterator CharactersMapIterator;


	Info mInfo;
	Common mCommon;
	std::map<unsigned int, Char*> mChars;
	std::string mTextureFilename;
	const ITexture* mTexture;

public:
	FontType();
	~FontType();

	std::string GetName() const;
	void SetName(const std::string& name);
	void SetSize(int size);

	void SetLineHeight(int height);
	void SetBase(int base);
	void SetScaleWidth(int width);
	void SetScaleHeight(int height);
	void SetTextureFilename(const std::string& filename);

	unsigned int GetNumCharacters() const;
	void AddCharacter(unsigned int id, Char* character);
	const FontType::Char* GetCharacter(unsigned int id);
	void SetTexture(const ITexture* texture);
	const ITexture* GetTexture() const;
	const std::string GetTextureFilename() const;
};

