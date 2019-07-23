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
	static const char* CUBE_NAME;
	static const char* SKYBOX_NAME;
	static const char* QUAD_NAME;
	static const char* GUI_QUAD_NAME;
	static const char* PARTICLE_QUAD_NAME;

	ModelsLibrary(TexturesLibrary* texturesLibrary, AnimationsLibrary* animationsLibrary);
	~ModelsLibrary();

	void Load();
	// Heredado v�a ISerializable
	virtual void ReadFrom(core::utils::IDeserializer* source) override;
	virtual void WriteTo(core::utils::ISerializer* destination) override;

private:
	void LoadModel(const std::string& name, const std::string& filename, bool calculateNormals, bool calculateTangents);
	void CreateCube();
	void CreateSkyBox();
	void CreateQuad(const std::string& name, float width, float height);

	//TODO este m�todo podr�a estar en una librer�a statica
	std::string GetPath(const std::string& filename);
};

