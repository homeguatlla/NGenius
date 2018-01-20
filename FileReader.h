#pragma once
#include <string>
#include <glm/glm.hpp>

class FileReader
{
public:
	static bool Read(std::string& line, const std::string prefix);
	static std::string ReadToken(std::string& line);
	static glm::vec3  ReadVertex3(const std::string& line);
	static glm::vec2  ReadVertex2(const std::string& line);
	static std::string GetFilepath(const std::string& filename);
};

