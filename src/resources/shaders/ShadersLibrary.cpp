#include "stdafx.h"
#include "ShadersLibrary.h"
#include "IShaderProgram.h"
#include "DefaultShader.h"
#include "TerrainShader.h"
#include "ModelShader.h"
#include "NormalMapShader.h"
#include "SkyBoxShader.h"
#include "GUIShader.h"
#include "WaterShader.h"
#include "ParticleShader.h"
#include "EnergyWallShader.h"
#include "TextShader.h"
#include "ShadowShader.h"
#include "OverdrawShader.h"
#include "ContrastShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "BrightShader.h"
#include "BloomShader.h"
#include "GrassShader.h"
#include "AnimatedModelShader.h"
#include "../InstantiableObject.h"
#include "../../utils/serializer/IDeserializer.h"
#include <string>
#include <iostream>


ShadersLibrary::ShadersLibrary()
{
	InstantiableObject::RegisterType("terrain", new TerrainShader());
	InstantiableObject::RegisterType("model", new ModelShader());
	InstantiableObject::RegisterType("normalmap", new NormalMapShader());
	InstantiableObject::RegisterType("skybox", new SkyBoxShader());
	InstantiableObject::RegisterType("particle", new ParticleShader());
	InstantiableObject::RegisterType("gui", new GUIShader());
	InstantiableObject::RegisterType("water", new WaterShader());
	InstantiableObject::RegisterType("energy_wall", new EnergyWallShader());
	InstantiableObject::RegisterType("text", new TextShader());
	InstantiableObject::RegisterType("shadow", new ShadowShader());
	InstantiableObject::RegisterType("contrast", new ContrastShader());
	InstantiableObject::RegisterType("horizontal_blur", new HorizontalBlurShader());
	InstantiableObject::RegisterType("vertical_blur", new VerticalBlurShader());
	InstantiableObject::RegisterType("bright", new BrightShader());
	InstantiableObject::RegisterType("bloom", new BloomShader());
	InstantiableObject::RegisterType("grass", new GrassShader());
	InstantiableObject::RegisterType("animated_model", new AnimatedModelShader());
}

ShadersLibrary::~ShadersLibrary()
{
}

void ShadersLibrary::Load()
{
	AddShader("default", new DefaultShader());
	AddShader("overdraw", new OverdrawShader());
}

void ShadersLibrary::AddShader(const char* name, IShaderProgram* shader)
{
	shader->Init();
	AddElement(name, shader);
}

void ShadersLibrary::ReadFrom(core::utils::IDeserializer * source)
{
	source->BeginAttribute(std::string("shaders_library"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute(std::string("shader"));
	do
	{
		std::string nodeName = source->GetCurrentNodeName();
		std::string name;
		source->ReadParameter("name", name);
		AddShader(name.c_str(), InstantiableObject::CreateShader(name));
		source->NextAttribute();
		numElements--;

	} while (numElements > 0);

	source->EndAttribute();
	source->EndAttribute();
}

void ShadersLibrary::WriteTo(core::utils::ISerializer * destination)
{
}
