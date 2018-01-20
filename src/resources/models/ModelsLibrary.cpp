#include "stdafx.h"
#include "ModelsLibrary.h"
#include "Model.h"
#include "../../loader/OBJLoader.h"

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
	Model* model = OBJLoader::LoadModel(filename);
	if (model != nullptr)
	{
		AddElement(name, model);

		if (!model->GetDiffuseTextureName().empty())
		{
			mTexturesLibrary->AddTextureNameToLoad(model->GetMaterialName() + "_diffuse", model->GetDiffuseTextureName());
		}

		if (!model->GetNormalMapTextureName().empty())
		{
			mTexturesLibrary->AddTextureNameToLoad(model->GetMaterialName() + "_normalmap", model->GetNormalMapTextureName());
		}
	}
}
