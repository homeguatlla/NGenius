#pragma once
#include <map>
#include "../BaseLibrary.h"
#include "../textures/TexturesLibrary.h"
#include "../models/animation/AnimatedModelLibrary.h"
#include "../models/animation/AnimationsLibrary.h"

class Model;

class ModelsLibrary : public BaseLibrary<Model*>
{
	TexturesLibrary* mTexturesLibrary;
	AnimationsLibrary* mAnimationsLibrary;
	AnimatedModelLibrary* mAnimatedModelLibrary;

public:
	ModelsLibrary(TexturesLibrary* texturesLibrary, AnimationsLibrary* animationsLibrary, AnimatedModelLibrary* animatedModelLibrary);
	~ModelsLibrary();

	void Load();

private:
	void LoadModel(const std::string& name, const std::string& filename, bool calculateNormals, bool calculateTangents);
	void CreateCube();
	void CreateSkyBox();
	void CreateQuad(const std::string& name, float width, float height);
};

