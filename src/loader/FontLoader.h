#pragma once
#include "../utils/FileReader.h"

class FontType;

class FontLoader : public FileReader
{
public:
	
	static FontType* LoadFont(const std::string& filename);
private:
	static void ReadToken(const std::string& token, std::string& name, std::string& value);
	static void ConsumeToken(std::string& line, std::string& token);
	static void ReadInfo(FontType* fontType, const std::string& line);
	static void ReadCommon(FontType* fontType, const std::string& line);
	static void ReadPage(FontType* fontType, const std::string& line);
	static int ReadChars(FontType* fontType, const std::string& line);
	static void ReadChar(FontType* fontType, const std::string& line);
};

