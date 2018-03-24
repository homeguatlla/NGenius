#include "stdafx.h"
#include "ModelsLibrary.h"
#include "Model.h"
#include "Mesh.h"
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
	CreateCube();
	CreateSkyBox();
	CreateQuad("quad");
	//necesitamos un gui_quad porque sino, cuando se construye (build) 
	//buscará el atributo color_gradient. Si no lo tiene, que para el shader_gui no lo tiene, 
	//después para las partículas no lo asignará y no les funcionará el color_gradient.
	//creando tres quads, ya es diferente la cosa.
	CreateQuad("gui_quad");
	CreateQuad("particle_quad");

	LoadModel("cube2", "data/models/cube/cube.obj", false, true);
	LoadModel("enano", "data/models/enano/enano.obj", false, true);
	LoadModel("mazo", "data/models/mazo/mazo.obj", false, true);

	LoadModel("barrel", "data/models/props/barrel.obj", true, true);
	LoadModel("chest", "data/models/props/chest.obj", false, true);
	LoadModel("brazier", "data/models/props/brazier.obj", false, true);
	LoadModel("stall", "data/models/stall/stall.obj", false, true);
	LoadModel("cube3", "data/models/props/cube.obj", false, true);
	LoadModel("crate", "data/models/props/crate.obj", true, true);
	LoadModel("floor", "data/models/props/floor.obj", true, true);
	LoadModel("statue", "data/models/props/statue.obj", true, true);

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
	Mesh* model = OBJLoader::LoadModel(filename);

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

void ModelsLibrary::CreateSkyBox()
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
	Mesh* mMesh = new Mesh(vertexs, uv, indexes);
	Model* model = new Model(mMesh);

	AddElement("skybox", model);
}

void ModelsLibrary::CreateCube()
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

	indexes.push_back(0);
	indexes.push_back(1);

	indexes.push_back(1);
	indexes.push_back(3);
	
	indexes.push_back(3);
	indexes.push_back(2);
	
	indexes.push_back(2);
	indexes.push_back(4);
	
	indexes.push_back(4);
	indexes.push_back(6);
	
	indexes.push_back(6);
	indexes.push_back(0); 
	
	indexes.push_back(6);
	indexes.push_back(7);
	
	indexes.push_back(7);
	indexes.push_back(1);

	indexes.push_back(7);
	indexes.push_back(5);

	indexes.push_back(5);
	indexes.push_back(3);

	indexes.push_back(5);
	indexes.push_back(4);

	indexes.push_back(0);
	indexes.push_back(2);

	std::vector<glm::vec2> uv;
	Mesh* mMesh = new Mesh(vertexs, uv, indexes);
	Model* model = new Model(mMesh);

	AddElement("cube", model);
}

void ModelsLibrary::CreateQuad(const std::string& name)
{
	std::vector<glm::vec3> vertexs;
	vertexs.push_back(glm::vec3(-0.5, 0.5, 0.0f));
	vertexs.push_back(glm::vec3(0.5, 0.5, 0.0f));
	vertexs.push_back(glm::vec3(0.5, -0.5, 0.0f));
	vertexs.push_back(glm::vec3(-0.5, -0.5, 0.0f));

	std::vector<glm::vec2> uv;
	uv.push_back(glm::vec2(0.0f, 1.0f));
	uv.push_back(glm::vec2(1.0f, 1.0f));
	uv.push_back(glm::vec2(1.0f, 0.0f));
	uv.push_back(glm::vec2(0.0f, 0.0f));

	std::vector<unsigned int> indexs;
	indexs.push_back(0);
	indexs.push_back(3);
	indexs.push_back(2);

	indexs.push_back(0);
	indexs.push_back(2);
	indexs.push_back(1);

	Mesh* mMesh = new Mesh(vertexs, uv, indexs);
	Model* model = new Model(mMesh);

	AddElement(name, model);
}