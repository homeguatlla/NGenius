#include "stdafx.h"
#include "FontsLibrary.h"

#include "../../loader/FontLoader.h"
#include "../textures/TexturesLibrary.h"
#include "../textures/ITexture.h"
#include "FontType.h"
#include "../../utils/serializer/IDeserializer.h"

FontsLibrary::FontsLibrary(TexturesLibrary* texturesLibrary) : mTexturesLibrary(texturesLibrary)
{
}


FontsLibrary::~FontsLibrary()
{
}

void FontsLibrary::Load()
{
	LoadFont("data/fonts/OCRAExtended.fnt");
	//LoadFont("data/fonts/quartzMS.fnt");
	//LoadFont("data/fonts/CourierNew.fnt");	
	//LoadFont("data/fonts/OCRAExtended_DField.fnt");
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

void FontsLibrary::OnTextureFontLoaded(const std::string& textureName, ITexture* texture)
{
	FontType* fontType = GetElement(textureName);
	assert(fontType != nullptr);
	if (fontType != nullptr)
	{
		fontType->SetTexture(texture);
	}
}

void FontsLibrary::ReadFrom(core::utils::IDeserializer *source)
{
	source->BeginAttribute(std::string("fonts_library"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute(std::string("font"));
	do
	{
		std::string nodeName = source->GetCurrentNodeName();
		std::string filename;
		source->ReadParameter("filename", filename);
		LoadFont(filename);
		source->NextAttribute();
		numElements--;

	} while (numElements > 0);

	source->EndAttribute();
	source->EndAttribute();
}

void FontsLibrary::WriteTo(core::utils::ISerializer *destination)
{
}
