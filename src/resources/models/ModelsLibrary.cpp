#include "stdafx.h"
#include "ModelsLibrary.h"
#include "Model.h"
#include "ModelGeometry.h"
#include "../../loader/OBJLoader.h"

#include <iostream>

ModelsLibrary::ModelsLibrary(TexturesLibrary* texturesLibrary) : mTexturesLibrary(texturesLibrary)
{
}


ModelsLibrary::~ModelsLibrary()
{
	
}

void ModelsLibrary::Load()
{
	LoadModel("cube", "data/models/cube/cube.obj");
	LoadModel("enano", "data/models/enano/enano.obj");
	LoadModel("mazo", "data/models/mazo/mazo.obj");

	LoadModel("barrel", "data/models/props/barrel.obj");
	LoadModel("chest", "data/models/props/chest.obj");
	LoadModel("brazier", "data/models/props/brazier.obj");
	LoadModel("stall", "data/models/stall/stall.obj");
	LoadModel("cube2", "data/models/props/cube.obj");

	//model = OBJLoader::LoadModel("data/models/hermes/hermes.obj");
	//AddElement("hermes", model);

	LoadModel("sphere", "data/models/sphere/sphere.obj");
	
	LoadModel("tree_foliage_0", "data/models/tree4/tree_foliage_lod0.obj");
	LoadModel("tree_foliage_1", "data/models/tree4/tree_foliage_lod1.obj");
	LoadModel("tree_foliage_2", "data/models/tree4/tree_foliage_lod2.obj");

	LoadModel("tree_trunk_0", "data/models/tree4/tree_trunk_lod0.obj");
	LoadModel("tree_trunk_1", "data/models/tree4/tree_trunk_lod1.obj");
	LoadModel("tree_trunk_2", "data/models/tree4/tree_trunk_lod2.obj");


	//LoadModel("marine", "data/models/marine/marine.obj");
	//LoadModel("stone", "data/models/stone/stone.obj");

	//model = OBJLoader::LoadModel("data/models/tree/tree.obj");
	//AddElement("tree", model);

	//model = OBJLoader::LoadModel("data/models/tree2/full019.obj");
	//AddElement("tree2", model);
}

void ModelsLibrary::LoadModel(const std::string& name, const std::string& filename)
{
	ModelGeometry* model = OBJLoader::LoadModel(filename);
	if (model != nullptr)
	{
		Model* modelRender = new Model(model);
		AddElement(name, modelRender);

		if (!model->GetDiffuseTextureName().empty())
		{
			mTexturesLibrary->AddTextureNameToLoad(model->GetMaterialName() + "_diffuse", model->GetDiffuseTextureName());
		}

		if (!model->GetNormalMapTextureName().empty())
		{
			mTexturesLibrary->AddTextureNameToLoad(model->GetMaterialName() + "_normalmap", model->GetNormalMapTextureName());
		}
	}
	else
	{
		std::cout << "Error reading model " << filename;
	}
}
