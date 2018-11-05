#include "stdafx.h"
#include "TextureArray.h"
#include "PNGLoader.h"
#include "../../utils/Log.h"
#include <GL/glew.h>
#include <iostream>
#include <assert.h>

TextureArray::TextureArray() : mNumberTextures(0)
{
}


TextureArray::~TextureArray()
{
}

void TextureArray::Load(std::vector<std::string>& filenames, unsigned int textureUnit, bool hasMipmapping)
{
	PNGLoader loader;
	int textureMaxWidth = 256;
	int textureMaxHeight = 256;
	mTextureUnit = textureUnit;
	mNumberTextures = filenames.size();

	glGenTextures(1, &mTextureID);
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureID);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, textureMaxWidth, textureMaxHeight, mNumberTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	for (unsigned int i = 0; i < mNumberTextures; ++i)
	{
		bool loaded = loader.ReadPNGFile(filenames[i].c_str());

		if (!loaded)
		{
			Log(Log::LOG_ERROR) << "Couldn't load texture %s " << filenames[i];
			return;
		}
		//the last value will be the remaining value for with height
		mWidth = loader.GetWidth();
		mHeight = loader.GetHeight();

		assert(loader.GetWidth() <= textureMaxWidth);
		assert(loader.GetHeight() <= textureMaxHeight);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, loader.GetWidth(), loader.GetHeight(), 1, GL_RGBA, GL_UNSIGNED_BYTE, loader.GetData());
	}
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (hasMipmapping)
	{
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS_EXT, -0.4f);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

unsigned int TextureArray::GetNumberTextures() const
{
	return mNumberTextures;
}