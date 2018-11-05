#pragma once
#include <map>
#include "../BaseLibrary.h"
#include "../textures/TexturesLibrary.h"
#include "../models/animation/AnimationsLibrary.h"

class Model;

class ModelsLibrary : public BaseLibrary<Model*>
{
	TexturesLibrary* mTexturesLibrary;
	AnimationsLibrary* mAnimationsLibrary;

public:
	ModelsLibrary(TexturesLibrary* texturesLibrary, AnimationsLibrary* animationsLibrary);
	~ModelsLibrary();

	void Load();

private:
	void LoadModel(const std::string& name, const std::string& filename, bool calculateNormals, bool calculateTangents);
	void CreateCube();
	void CreateSkyBox();
	void CreateQuad(const std::string& name, float width, float height);

	//TODO este método podría estar en una librería statica
	std::string GetPath(const std::string& filename);
};

