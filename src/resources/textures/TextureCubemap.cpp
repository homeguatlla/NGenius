#include "stdafx.h"
#include "TextureCubemap.h"
#include "PNGLoader.h"
#include "../../utils/Log.h"
#include <GL/glew.h>
#include <iostream>
#include <assert.h>

TextureCubemap::TextureCubemap() : mNumberTextures(6)
{
}


TextureCubemap::~TextureCubemap()
{
}

void TextureCubemap::Load(std::vector<std::string>& filenames, int textureUnit)
{
	PNGLoader loader;
	
	//assert(filenames.size() == 6);

	glGenTextures(1, &mTextureID);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

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

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, loader.GetWidth(), loader.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loader.GetData());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

unsigned int TextureCubemap::GetNumberTextures() const
{
	return mNumberTextures;
}