#include "stdafx.h"
#include "ITexture.h"
#include <GL/glew.h>
#include <assert.h>

ITexture::ITexture() : mTextureID(0), mTextureUnit(0), mWidth(0), mHeight(0)
{
}


ITexture::~ITexture()
{
}

unsigned int ITexture::GetID() const
{
	return mTextureID;
}

unsigned int ITexture::GetUnit() const
{
	assert(mTextureUnit >= GL_TEXTURE0 && mTextureUnit <= GL_TEXTURE31);
	return mTextureUnit;
}

unsigned int ITexture::GetWidth() const
{
	return mWidth;
}

unsigned int ITexture::GetHeight() const
{
	return mHeight;
}

void ITexture::SetActive(bool active, unsigned int unit)
{
	assert(unit >= GL_TEXTURE0 && unit <= GL_TEXTURE31);

	if (active)
	{
		mTextureUnit = unit;
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glBindTextureUnit(unit, mTextureID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
