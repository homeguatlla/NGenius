#include "stdafx.h"
#include "Texture.h"
#include <GL/glew.h>
#include <iostream>

Texture::Texture()
{
}


Texture::~Texture()
{
	glDeleteTextures(1, &mTextureID);
}

bool Texture::Load(const std::string& filename, unsigned int textureUnit, bool hasMipmapping, bool hasWrapping)
{
	mTextureUnit = textureUnit;
	bool loaded = mLoader.ReadPNGFile(filename.c_str());

	if (!loaded)
	{
		std::cout << "Couldn't load texture %s " << filename << "\n";
		return false;
	}

	mWidth = mLoader.GetWidth();
	mHeight = mLoader.GetHeight();

	glGenTextures(1, &mTextureID);
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLoader.GetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hasWrapping ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, hasWrapping ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (hasMipmapping)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS_EXT, -0.4f);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::CreateTexture(int textureUnit, int width, int height)
{
	mTextureUnit = textureUnit;
	mWidth = width;
	mHeight = height;

	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::CreateDepthTexture(int textureUnit, int width, int height)
{
	mTextureUnit = textureUnit;
	mWidth = width;
	mHeight = height;

	glGenTextures(1, &mTextureID);
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec4 Texture::GetColor(glm::vec2 uv) const
{
	int u = static_cast<int>(uv.x * mLoader.GetWidth());
	int v = static_cast<int>(uv.y * mLoader.GetHeight());
	return glm::vec4(mLoader.GetR(u, v), mLoader.GetG(u, v), mLoader.GetB(u, v), mLoader.GetA(u, v));
}