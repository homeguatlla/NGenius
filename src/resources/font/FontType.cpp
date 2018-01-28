#include "stdafx.h"
#include "FontType.h"

#include <assert.h>

FontType::FontType()
{
}

FontType::~FontType()
{ 
	//Deleting all characters
	for (CharactersMapIterator it = mChars.begin(); it != mChars.end(); ++it)
	{
		delete it->second;
	}

	mChars.clear();
}

std::string FontType::GetName() const
{
	return mInfo.face;
}

void FontType::SetName(const std::string& name)
{
	mInfo.face = name;
}

void FontType::SetSize(int size)
{
	mInfo.size = size;
}

void FontType::SetLineHeight(int height)
{
	mCommon.lineHeight = height;
}

void FontType::SetBase(int base)
{
	mCommon.base = base;
}

void FontType::SetScaleWidth(int width)
{
	mCommon.scaleWidth = width;
}

void FontType::SetScaleHeight(int height)
{
	mCommon.scaleHeight = height;
}

void FontType::SetTextureFilename(const std::string& filename)
{
	mTextureFilename = filename;
}

unsigned int FontType::GetNumCharacters() const
{
	return mChars.size();
}

void FontType::AddCharacter(unsigned int id, Char* character)
{
	assert(character != nullptr);

	if (character != nullptr)
	{
		mChars[id] = character;
	}
}

const FontType::Char* FontType::GetCharacter(unsigned int id)
{
	return mChars[id];
}

void FontType::SetTexture(const ITexture* texture)
{
	mTexture = texture;
}

const ITexture* FontType::GetTexture() const
{
	return mTexture;
}

const std::string FontType::GetTextureFilename() const
{
	return mTextureFilename;
}
