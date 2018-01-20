#include "stdafx.h"
#include "FontsLibrary.h"

#include "../../loader/FontLoader.h"
#include "../textures/TexturesLibrary.h"
#include "../textures/ITexture.h"
#include "FontType.h"

FontsLibrary::FontsLibrary(TexturesLibrary* texturesLibrary) : mTexturesLibrary(texturesLibrary)
{
}


FontsLibrary::~FontsLibrary()
{
}

void FontsLibrary::Load()
{
	LoadFont("data/fonts/quartzMS.fnt");
	LoadFont("data/fonts/CourierNew.fnt");
	LoadFont("data/fonts/OCRAExtended.fnt");
	LoadFont("data/fonts/OCRAExtended_DField.fnt");
}

void FontsLibrary::LoadFont(const std::string& filename)
{
	FontType* font = FontLoader::LoadFont(filename);
	std::string filepath = FileReader::GetFilepath(filename);
	if (font != nullptr)
	{
		AddElement(font->GetName(), font);
		if (!font->GetTextureFilename().empty())
		{
			mTexturesLibrary->AddTextureNameToLoad(font->GetName(), filepath + font->GetTextureFilename(), std::bind(&FontsLibrary::OnTextureFontLoaded, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
}

void FontsLibrary::OnTextureFontLoaded(const std::string& textureName, const ITexture* texture)
{
	FontType* fontType = GetElement(textureName);
	assert(fontType != nullptr);
	if (fontType != nullptr)
	{
		fontType->SetTexture(texture);
	}
}