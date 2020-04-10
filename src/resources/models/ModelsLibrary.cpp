#include "stdafx.h"
#include "ModelsLibrary.h"
#include "Model.h"
#include "Mesh.h"
#include "../../loader/ModelFactory.h"
#include "animation/AnimatedModel.h"
#include "animation/Animation.h"

#include "../../utils/serializer/IDeserializer.h"
#include "../../utils/Log.h"
#include "../Memory.h"

#include <iostream>

const char* ModelsLibrary::CUBE_NAME = "cube_default";
const char* ModelsLibrary::SPHERE_NAME = "sphere_default";
const char* ModelsLibrary::SKYBOX_NAME = "skybox";
const char* ModelsLibrary::QUAD_NAME = "quad_default";
const char* ModelsLibrary::GUI_QUAD_NAME = "gui_quad";
const char* ModelsLibrary::PARTICLE_QUAD_NAME = "particle_quad";

ModelsLibrary::ModelsLibrary(TexturesLibrary* texturesLibrary, AnimationsLibrary* animationsLibrary) : 
	mTexturesLibrary(texturesLibrary),
	mAnimationsLibrary(animationsLibrary)
{
}

ModelsLibrary::~ModelsLibrary()
{
	
}

void ModelsLibrary::Load()
{
	CreateCube();
	CreateSkyBox();
	CreateQuad(ModelsLibrary::QUAD_NAME, 1.0f, 1.0f);
	//necesitamos un gui_quad porque sino, cuando se construye (build) 
	//buscará el atributo color_gradient. Si no lo tiene, que para el shader_gui no lo tiene, 
	//después para las partículas no lo asignará y no les funcionará el color_gradient.
	//creando tres quads, ya es diferente la cosa.
	CreateQuad(ModelsLibrary::GUI_QUAD_NAME, 0.5f, 0.5f);
	CreateQuad(ModelsLibrary::PARTICLE_QUAD_NAME, 0.5f, 0.5f);
	
	LoadModel(ModelsLibrary::SPHERE_NAME, "data/models/sphere/sphere.obj", true, true);

	//LoadModel("cube2", "data/models/cube/cube.obj", false, true);
	//LoadModel("enano", "data/models/enano/enano.obj", false, true);
	//LoadModel("mazo", "data/models/mazo/mazo.obj", false, true);

	///LoadModel("barrel", "data/models/props/barrel.obj", true, true);
	///LoadModel("chest", "data/models/props/chest.obj", true, true);
///	LoadModel("brazier", "data/models/props/brazier.obj", true, true);
	//LoadModel("stall", "data/models/stall/stall.obj", false, true);
	//LoadModel("cube3", "data/models/props/cube.obj", false, true);
	///LoadModel("crate", "data/models/props/crate.obj", true, true);
	//LoadModel("floor", "data/models/props/floor.obj", true, true);
	///LoadModel("statue", "data/models/props/statue.obj", true, true);

	///LoadModel("sphere", "data/models/sphere/sphere.obj", false, true);

	/*LoadModel("tree_foliage_0", "data/models/tree4/tree_foliage_lod0.obj", false, true);
	LoadModel("tree_foliage_1", "data/models/tree4/tree_foliage_lod1.obj", false, true);
	LoadModel("tree_foliage_2", "data/models/tree4/tree_foliage_lod2.obj", false, true);

	LoadModel("tree_trunk_0", "data/models/tree4/tree_trunk_lod0.obj", false, true);
	LoadModel("tree_trunk_1", "data/models/tree4/tree_trunk_lod1.obj", false, true);
	LoadModel("tree_trunk_2", "data/models/tree4/tree_trunk_lod2.obj", false, true);
	*/
	///LoadModel("farmer", "data/models/farmer/farmer.dae", true, true);

	//LoadModel("grass1", "data/models/grass/grass1.obj", false, false);
	//LoadModel("grass2", "data/models/grass/grass2.obj", false, false);

	//model = OBJLoader::LoadModel("data/models/hermes/hermes.obj");
	//AddElement("hermes", model);
	
	
	//LoadModel("farmer", "data/models/cube/cube.dae", false, true);
	//LoadModel("farmer", "data/models/Adventurer-Militia/Militia-Adventurer-RIGGED.dae", false, true);

	//LoadModel("farmer", "data/models/ethan/model.dae", false, true);
	//LoadModel("barrel2", "data/models/barrel/barrel.obj", false, true);
	//LoadModel("wooden", "data/models/wooden/wood.fbx", false, true);
	//LoadModel("bridge", "data/models/wooden_bridge/wooden_bridge.fbx", false, true);


	//LoadModel("cubeSimple", "data/models/cube/untitled.dae", false, true);

	//LoadModel("cube_animated", "data/models/cube/cube.fbx", false, true);
	/*
	LoadModel("yurown", "data/models/yurown/Sci-Fi_Soldier.fbx", false, true);
	LoadModel("spider", "data/models/spider2/spider.fbx", false, true);
	*/
	//LoadModel("marine", "data/models/marine/marine.obj", true, true);
	//LoadModel("stone", "data/models/stone/stone.obj");

	//model = OBJLoader::LoadModel("data/models/tree/tree.obj");
	//AddElement("tree", model);

	//model = OBJLoader::LoadModel("data/models/tree2/full019.obj");
	//AddElement("tree2", model);
}

void ModelsLibrary::LoadModel(const std::string& name, const std::string& filename, bool calculateNormals, bool calculateTangents)
{
	Animation* animation = nullptr;
	Joint* rootJoint = nullptr;

	Mesh* model = ModelFactory::LoadModel(filename, &animation, &rootJoint);

	if (model != nullptr)
	{
		model->Build(calculateNormals, calculateTangents);

		if (model->GetMaterialName().empty())
		{
			model->SetMaterialName("material_" + name);
		}

		if (animation != nullptr && rootJoint != nullptr)
		{
			AnimatedModel* animatedModel = DBG_NEW AnimatedModel(name, model, rootJoint);
			AddOrReplaceElement(name, animatedModel);
			mAnimationsLibrary->AddOrReplaceElement(animation->GetName(), animation);
		}
		else
		{
			Model* modelRender = DBG_NEW Model(name, model);
			AddOrReplaceElement(name, modelRender);
		}

		std::string path = GetPath(filename) + "/";

		if (!model->GetDiffuseTextureName().empty())
		{
			mTexturesLibrary->AddTextureNameToLoad(model->GetMaterialName() + "_diffuse", path + model->GetDiffuseTextureName());
		}

		if (!model->GetNormalMapTextureName().empty())
		{
			mTexturesLibrary->AddTextureNameToLoad(model->GetMaterialName() + "_normalmap", path + model->GetNormalMapTextureName());
		}
	}
	else
	{
		Log(Log::LOG_ERROR) << "Error reading model " << filename;
	}
}

std::string ModelsLibrary::GetPath(const std::string& filename)
{
	char separator[2] = { '/', '\\' };
	
	for (int s = 0; s < 2; ++s)
	{
		size_t i = filename.rfind(separator[s], filename.length());
		if (i != std::string::npos)
		{
			return(filename.substr(0, i));
		}
	}
	
	return "";
}

void ModelsLibrary::ReadFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute(std::string("models_library"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute("model");
	do
	{	
		std::string filename;
		source->ReadParameter("filename", filename);

		std::string modelName;
		source->ReadParameter("name", modelName);

		bool calculateNormals = false;
		bool calculateTangents = false;
		source->ReadParameter("calculate_normals", &calculateNormals);
		source->ReadParameter("calculate_tangents", &calculateTangents);

		LoadModel(modelName, filename, calculateNormals, calculateTangents);
		
		source->NextAttribute();
		numElements--;
	} while (numElements > 0);
	source->EndAttribute();
	source->EndAttribute();
}

void ModelsLibrary::WriteTo(core::utils::ISerializer* destination)
{
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
	Mesh* mMesh = DBG_NEW Mesh(vertexs, uv, indexes);
	Model* model = DBG_NEW Model("skybox", mMesh);

	AddOrReplaceElement(ModelsLibrary::SKYBOX_NAME, model);
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
	Mesh* mMesh = DBG_NEW Mesh(vertexs, uv, indexes);
	Model* model = DBG_NEW Model(CUBE_NAME, mMesh);

	AddOrReplaceElement(ModelsLibrary::CUBE_NAME, model);
}

void ModelsLibrary::CreateQuad(const std::string& name, float width, float height)
{
	std::vector<glm::vec3> vertexs;
	vertexs.push_back(glm::vec3(-width, height, 0.0f));
	vertexs.push_back(glm::vec3(width, height, 0.0f));
	vertexs.push_back(glm::vec3(width, -height, 0.0f));
	vertexs.push_back(glm::vec3(-width, -height, 0.0f));

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

	Mesh* mMesh = DBG_NEW Mesh(vertexs, uv, indexs);
	Model* model = DBG_NEW Model(QUAD_NAME, mMesh);

	AddOrReplaceElement(name, model);
}