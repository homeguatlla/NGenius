#include "stdafx.h"
#include "ITexture.h"
#include <GL/glew.h>

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

void ITexture::SetActive(bool active)
{
	if (active)
	{
		glActiveTexture(GL_TEXTURE0 + mTextureUnit);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
