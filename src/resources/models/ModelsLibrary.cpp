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
	CreateSkybox();
	LoadModel("cube2", "data/models/cube/cube.obj", false, true);
	LoadModel("enano", "data/models/enano/enano.obj", false, true);
	LoadModel("mazo", "data/models/mazo/mazo.obj", false, true);

	LoadModel("barrel", "data/models/props/barrel.obj", true, true);
	LoadModel("chest", "data/models/props/chest.obj", false, true);
	LoadModel("brazier", "data/models/props/brazier.obj", false, true);
	LoadModel("stall", "data/models/stall/stall.obj", false, true);
	LoadModel("cube3", "data/models/props/cube.obj", false, true);

	//model = OBJLoader::LoadModel("data/models/hermes/hermes.obj");
	//AddElement("hermes", model);

	LoadModel("sphere", "data/models/sphere/sphere.obj", false, true);
	
	LoadModel("tree_foliage_0", "data/models/tree4/tree_foliage_lod0.obj", false, true);
	LoadModel("tree_foliage_1", "data/models/tree4/tree_foliage_lod1.obj", false, true);
	LoadModel("tree_foliage_2", "data/models/tree4/tree_foliage_lod2.obj", false, true);

	LoadModel("tree_trunk_0", "data/models/tree4/tree_trunk_lod0.obj", false, true);
	LoadModel("tree_trunk_1", "data/models/tree4/tree_trunk_lod1.obj", false, true);
	LoadModel("tree_trunk_2", "data/models/tree4/tree_trunk_lod2.obj", false, true);


	//LoadModel("marine", "data/models/marine/marine.obj", true, true);
	//LoadModel("stone", "data/models/stone/stone.obj");

	//model = OBJLoader::LoadModel("data/models/tree/tree.obj");
	//AddElement("tree", model);

	//model = OBJLoader::LoadModel("data/models/tree2/full019.obj");
	//AddElement("tree2", model);
}

void ModelsLibrary::LoadModel(const std::string& name, const std::string& filename, bool calculateNormals, bool calculateTangents)
{
	ModelGeometry* model = OBJLoader::LoadModel(filename);

	model->Build(calculateNormals, calculateTangents);

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


void ModelsLibrary::CreateSkybox()
{
	float size = 0.5;
	std::vector<glm::vec3> vertexs;

	vertexs.push_back(glm::vec3(-size, -size, -size));
	vertexs.push_back(glm::vec3(-size, size, -size));
	vertexs.push_back(glm::vec3(size, -size, -size));
	vertexs.push_back(glm::vec3(size, size, -size));
	vertexs.push_back(glm::vec3(size, -size, size));
	vertexs.push_back(glm::vec3(size, size, size));
	vertexs.push_back(glm::vec3(-size, -size, size));
	vertexs.push_back(glm::vec3(-size, size, size));

	std::vector<unsigned int> indexes;

	indexes.push_back(2);
	indexes.push_back(1);
	indexes.push_back(0);

	indexes.push_back(3);
	indexes.push_back(1);
	indexes.push_back(2);

	indexes.push_back(4);
	indexes.push_back(3);
	indexes.push_back(2);

	indexes.push_back(5);
	indexes.push_back(3);
	indexes.push_back(4);

	indexes.push_back(6);
	indexes.push_back(5);
	indexes.push_back(4);

	indexes.push_back(7);
	indexes.push_back(5);
	indexes.push_back(6);

	indexes.push_back(0);
	indexes.push_back(7);
	indexes.push_back(6);

	indexes.push_back(1);
	indexes.push_back(7);
	indexes.push_back(0);

	indexes.push_back(2);
	indexes.push_back(0);
	indexes.push_back(6);

	indexes.push_back(6);
	indexes.push_back(4);
	indexes.push_back(2);

	indexes.push_back(3);
	indexes.push_back(5);
	indexes.push_back(7);

	indexes.push_back(1);
	indexes.push_back(3);
	indexes.push_back(7);
	
	std::vector<glm::vec2> uv;
	ModelGeometry* modelGeometry = new ModelGeometry(vertexs, uv, indexes);
	Model* model = new Model(modelGeometry);

	AddElement("skybox", model);
}