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
	InstantiableObject::RegisterShaderType<TerrainShader>();
	InstantiableObject::RegisterShaderType<ModelShader>();
	InstantiableObject::RegisterShaderType<NormalMapShader>();
	InstantiableObject::RegisterShaderType<SkyBoxShader>();
	InstantiableObject::RegisterShaderType<ParticleShader>();
	InstantiableObject::RegisterShaderType<WaterShader>();
	InstantiableObject::RegisterShaderType<EnergyWallShader>();
	InstantiableObject::RegisterShaderType<ContrastShader>();
	InstantiableObject::RegisterShaderType<HorizontalBlurShader>();
	InstantiableObject::RegisterShaderType<VerticalBlurShader>();
	InstantiableObject::RegisterShaderType<BrightShader>();
	InstantiableObject::RegisterShaderType<BloomShader>();
	InstantiableObject::RegisterShaderType<GrassShader>();
	InstantiableObject::RegisterShaderType<AnimatedModelShader>();
}

void ShadersLibrary::Load()
{
	AddShader("DefaultShader", new DefaultShader());
	AddShader("OverdrawShader", new OverdrawShader());
	AddShader("TextShader", new TextShader());
	AddShader("ShadowShader", new ShadowShader());
	AddShader("GUIShader", new GUIShader());
	AddShader("NormalMapShader", new NormalMapShader());
}

void ShadersLibrary::AddShader(const char* name, IShaderProgram* shader)
{
	shader->Init();
	AddOrReplaceElement(name, shader);
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
