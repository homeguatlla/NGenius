#pragma once
#include <string>
#include <vector>
#include "ITexture.h"

class TextureCubemap : public ITexture
{
	unsigned int mNumberTextures;

public:
	TextureCubemap();
	~TextureCubemap();

	void Load(std::vector<std::string>& filenames, unsigned int textureUnit);
	unsigned int GetNumberTextures() const;
};

