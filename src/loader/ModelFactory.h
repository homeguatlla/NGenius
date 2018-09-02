#pragma once
#include <string>

class Mesh;

class ModelFactory
{
	enum class FileType
	{
		TYPE_OBJ,
		TYPE_FBX,
		TYPE_INVALID
	};

public:
	static Mesh* LoadModel(const std::string& filename);

private:
	static FileType GetFileType(const std::string& filename);
};

