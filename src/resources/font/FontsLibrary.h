#pragma once
#include "../BaseLibrary.h"
#include "../../utils/serializer/ISerializable.h"

class FontType;
class TexturesLibrary;
class ITexture;

class FontsLibrary : public core::utils::ISerializable, public BaseLibrary<FontType*>
{
	TexturesLibrary* mTexturesLibrary;

public:
	FontsLibrary(TexturesLibrary* texturesLibrary);
	~FontsLibrary();

	void Load();
	
	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer * source) override;
	virtual void WriteTo(core::utils::ISerializer * destination) override;

private:
	void LoadFont(const std::string& filename);
	void OnTextureFontLoaded(const std::string& textureName, ITexture* texture);
};

