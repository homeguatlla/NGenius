#pragma once
#include "../BaseLibrary.h"

class FontType;
class TexturesLibrary;
class ITexture;

class FontsLibrary : public BaseLibrary<FontType*>
{
	TexturesLibrary* mTexturesLibrary;

public:
	FontsLibrary(TexturesLibrary* texturesLibrary);
	~FontsLibrary();

	void Load();
	
private:
	void LoadFont(const std::string& filename);
	void OnTextureFontLoaded(const std::string& textureName, ITexture* texture);
};

