#pragma once
#include <string>
#include <vector>
#include "ITexture.h"

class TextureArray : public ITexture
{
	unsigned int mNumberTextures;

public:
	TextureArray();
	~TextureArray();

	void Load(std::vector<std::string>& filenames, unsigned int textureUnit, bool hasMipmapping);
	unsigned int GetNumberTextures() const;
};

