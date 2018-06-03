#include "stdafx.h"
#include "FileReader.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

static inline void ltrim(std::string &s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

std::string FileReader::GetFilepath(const std::string& filename)
{
	int index = filename.find("/", 0);
	while (index != std::string::npos)
	{
		int newIndex = filename.find("/", index + 1);
		if (newIndex == std::string::npos)
		{
			break;
		}
		else
		{
			index = newIndex;
		}
	}

	return filename.substr(0, index) + "/";
}

bool FileReader::Read(std::string& line, const std::string prefix)
{
	if (!line.empty() && line.find(prefix) != std::string::npos)
	{
		line = line.substr(prefix.size(), line.size() - prefix.size());
		ltrim(line);

		return true;
	}
	else
	{
		return false;
	}
}

std::string FileReader::ReadToken(std::string& line)
{
	std::string token;
	bool found = false;
	bool hasMarks = false;
	unsigned int i = 0;

	ltrim(line);

	while (!found && i < line.size())
	{
		switch (line[i])
		{
			case '\n':
				found = true;
				break;
			case '\"':
			{
				hasMarks = !hasMarks;
				token.push_back(line[i]);
				break;
			}				
			case ' ':
				if (hasMarks)
				{
					token.push_back(line[i]);
				}
				else
				{
					found = true;
				}
				break;
			default:
				token.push_back(line[i]);
				break;
		}
		++i;
	}
	return token;
}

glm::vec3 FileReader::ReadVertex3(const std::string& line)
{
	std::stringstream stream(line);
	float x, y, z;

	stream >> x;
	stream >> y;
	stream >> z;

	return glm::vec3(x, y, z);
}

glm::vec2 FileReader::ReadVertex2(const std::string& line)
{
	std::stringstream stream(line);
	float x, y;

	stream >> x;
	stream >> y;

	return glm::vec2(x, y);
}
