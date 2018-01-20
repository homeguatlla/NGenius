#include "stdafx.h"
#include "FontLoader.h"
#include "../resources/font/FontType.h"
#include <assert.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


FontType* FontLoader::LoadFont(const std::string& filename)
{
	std::ifstream input(filename);

	if (input.is_open())
	{
		FontType* fontType = new FontType();

		std::string line;
		unsigned int numCharacters = 0;

		while (std::getline(input, line))
		{
			if (Read(line, "info"))
			{
				ReadInfo(fontType, line);
			}
			else if (Read(line, "common"))
			{
				ReadCommon(fontType, line);
			}
			else if (Read(line, "chars"))
			{
				numCharacters = ReadChars(fontType, line);
			}
			else if (Read(line, "char"))
			{
				ReadChar(fontType, line);
			}
			else if (Read(line, "page"))
			{
				ReadPage(fontType, line);
			}
		}
		assert(numCharacters == fontType->GetNumCharacters());

		return fontType;
	}

	return nullptr;
}

void FontLoader::ReadInfo(FontType* fontType, const std::string& line)
{
	std::string newLine = line;
	std::string token = FileReader::ReadToken(newLine);

	std::string name;
	std::string value;

	//READ FACE
	ReadToken(token, name, value);
	if (name.compare("face") == 0)
	{
		value.erase(remove(value.begin(), value.end(), '"'), value.end());
		fontType->SetName(value);
	}
	ConsumeToken(newLine, token);

	//READ SIZE
	token = FileReader::ReadToken(newLine);
	ReadToken(token, name, value);
	if (name.compare("size") == 0)
	{
		fontType->SetSize(std::stoi(value));
	}
	ConsumeToken(newLine, token);
}

void FontLoader::ReadCommon(FontType* fontType, const std::string& line)
{
	std::string newLine = line;
	std::string token = FileReader::ReadToken(newLine);

	std::string name;
	std::string value;

	//READ LINE HEIGHT
	ReadToken(token, name, value);
	if (name.compare("lineHeight") == 0)
	{
		fontType->SetLineHeight(std::stoi(value));
	}
	ConsumeToken(newLine, token);
	
	//READ BASE
	token = FileReader::ReadToken(newLine);
	ReadToken(token, name, value);
	if (name.compare("base") == 0)
	{
		fontType->SetBase(std::stoi(value));
	}
	ConsumeToken(newLine, token);

	//READ scale width
	token = FileReader::ReadToken(newLine);
	ReadToken(token, name, value);
	if (name.compare("scaleW") == 0)
	{
		fontType->SetScaleWidth(std::stoi(value));
	}
	ConsumeToken(newLine, token);

	//READ scale height
	token = FileReader::ReadToken(newLine);
	ReadToken(token, name, value);
	if (name.compare("scaleH") == 0)
	{
		fontType->SetScaleHeight(std::stoi(value));
	}
	ConsumeToken(newLine, token);
}

void FontLoader::ReadPage(FontType* fontType, const std::string& line)
{
	std::string newLine = line;
	std::string token = FileReader::ReadToken(newLine);

	std::string name;
	std::string value;

	//READ id
	ReadToken(token, name, value);

	//READ texture filename
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);
	ReadToken(token, name, value);
	if (name.compare("file") == 0)
	{
		value.erase(remove(value.begin(), value.end(), '"'), value.end());
		fontType->SetTextureFilename(value);
	}
	ConsumeToken(newLine, token);
}

int FontLoader::ReadChars(FontType* fontType, const std::string& line)
{
	std::string newLine = line;
	std::string token = FileReader::ReadToken(newLine);

	std::string name;
	std::string value;

	//READ number of chars
	ReadToken(token, name, value);
	ConsumeToken(newLine, token);

	return std::stoi(value);
}

void FontLoader::ReadChar(FontType* fontType, const std::string& line)
{
	std::string newLine = line;
	std::string token = FileReader::ReadToken(newLine);

	std::string name;
	std::string value;

	FontType::Char* character = new FontType::Char();

	//READ ID
	ReadToken(token, name, value);
	unsigned int id = std::stoi(value);

	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	//READ x
	ReadToken(token, name, value);
	character->x = std::stof(value);
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	//READ y
	ReadToken(token, name, value);
	character->y = std::stof(value);
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	//READ width
	ReadToken(token, name, value);
	character->width = std::stof(value);
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	//READ height
	ReadToken(token, name, value);
	character->height = std::stof(value);
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	//READ xOffset
	ReadToken(token, name, value);
	character->xOffest = std::stoi(value);
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	//READ yOffset
	ReadToken(token, name, value);
	character->yOffset = std::stoi(value);
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	//READ xAdvance
	ReadToken(token, name, value);
	character->xAdvance = std::stoi(value);
	ConsumeToken(newLine, token);
	token = FileReader::ReadToken(newLine);

	fontType->AddCharacter(id, character);
}

void FontLoader::ReadToken(const std::string& token, std::string& name, std::string& value)
{
	std::size_t pos = token.find("=");
	name = token.substr(0, pos);
	value = token.substr(pos + 1);
}

void FontLoader::ConsumeToken(std::string& line, std::string& token)
{
	line = line.substr(token.size());
}