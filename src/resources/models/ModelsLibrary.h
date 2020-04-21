#pragma once
#include <map>
#include "../BaseLibrary.h"
#include "../textures/TexturesLibrary.h"
#include "../models/animation/AnimationsLibrary.h"
#include "../../utils/serializer/ISerializable.h"

class Model;

class ModelsLibrary : public core::utils::ISerializable, public BaseLibrary<Model*>
{
	TexturesLibrary* mTexturesLibrary;
	AnimationsLibrary* mAnimationsLibrary;
	

public:
	static const char* CUBE_WIREFRAME_NAME;
	static const char* CUBE_NAME;
	static const char* SPHERE_NAME;
	static const char* SKYBOX_NAME;
	static const char* QUAD_NAME;
	static const char* GUI_QUAD_NAME;
	static const char* PARTICLE_QUAD_NAME;

	ModelsLibrary(TexturesLibrary* texturesLibrary, AnimationsLibrary* animationsLibrary);
	~ModelsLibrary();

	void Load();
	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer* source) override;
	virtual void WriteTo(core::utils::ISerializer* destination) override;

private:
	void LoadModel(const std::string& name, const std::string& filename, bool calculateNormals, bool calculateTangents);
	void CreateCube();
	void CreateCubeWireframe();
	void CreateSkyBox();
	void CreateQuad(const std::string& name, float width, float height);
	void CreateGeometryForACube(float size, std::vector<glm::vec3>& vertexs, std::vector<unsigned int>& indexes, bool inside);

	//TODO este método podría estar en una librería statica
	std::string GetPath(const std::string& filename);
};

