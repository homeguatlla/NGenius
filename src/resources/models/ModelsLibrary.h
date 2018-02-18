#pragma once
#include <map>
#include "../BaseLibrary.h"
#include "../textures/TexturesLibrary.h"

class Model;

class ModelsLibrary : public BaseLibrary<Model*>
{
	TexturesLibrary* mTexturesLibrary;

public:
	ModelsLibrary(TexturesLibrary* texturesLibrary);
	~ModelsLibrary();

	void Load();

private:
	void LoadModel(const std::string& name, const std::string& filename, bool calculateNormals, bool calculateTangents);
	void CreateSkybox();
};

