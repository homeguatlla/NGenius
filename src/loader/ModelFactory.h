#pragma once
#include <string>

class Mesh;
class Animation;
class Joint;

class ModelFactory
{
	enum class FileType
	{
		TYPE_OBJ,
		TYPE_FBX,
		TYPE_INVALID
	};

public:
	static Mesh* LoadModel(const std::string& filename, Animation** animation = nullptr, Joint** rootJoint = nullptr);

private:
	static FileType GetFileType(const std::string& filename);
};

