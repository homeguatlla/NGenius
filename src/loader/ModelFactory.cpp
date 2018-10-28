#include "stdafx.h"
#include "ModelFactory.h"
#include "OBJLoader.h"
#include "FBXLoader.h"
#include "ColladaLoader.h"
#include "AssimpLoader.h"

#include <algorithm>

Mesh* ModelFactory::LoadModel(const std::string& filename, Animation** animation, Joint** rootJoint)
{
	FileType type = GetFileType(filename);

	switch (type)
	{
		case FileType::TYPE_OBJ:
			return OBJLoader::LoadModel(filename);
		case FileType::TYPE_FBX:
			return FBXLoader::LoadModel(filename, animation, rootJoint);
		case FileType::TYPE_COLLADA:
		{
			//return ColladaLoader::LoadModel(filename, animation, rootJoint);
			return AssimpLoader::LoadModel(filename, animation, rootJoint);
		}
		case FileType::TYPE_INVALID:
		default:
			return nullptr;
	}
}

ModelFactory::FileType ModelFactory::GetFileType(const std::string& filename)
{
	std::string upperString(filename);
	std::transform(upperString.begin(), upperString.end(), upperString.begin(), ::toupper);

	if (upperString.find(".OBJ") != std::string::npos)
	{
		return FileType::TYPE_OBJ;
	}
	else if (upperString.find(".FBX") != std::string::npos)
	{
		return FileType::TYPE_FBX;
	}
	else if (upperString.find(".DAE") != std::string::npos)
	{
		return FileType::TYPE_COLLADA;
	}
	else
	{
		return FileType::TYPE_INVALID;
	}
}