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
	InstantiableObject::RegisterType("s_terrain", new TerrainShader());
	InstantiableObject::RegisterType("s_model", new ModelShader());
	InstantiableObject::RegisterType("s_normalmap", new NormalMapShader());
	InstantiableObject::RegisterType("s_skybox", new SkyBoxShader());
	InstantiableObject::RegisterType("s_particle", new ParticleShader());
	InstantiableObject::RegisterType("s_water", new WaterShader());
	InstantiableObject::RegisterType("s_energy_wall", new EnergyWallShader());
	InstantiableObject::RegisterType("s_contrast", new ContrastShader());
	InstantiableObject::RegisterType("s_horizontal_blur", new HorizontalBlurShader());
	InstantiableObject::RegisterType("s_vertical_blur", new VerticalBlurShader());
	InstantiableObject::RegisterType("s_bright", new BrightShader());
	InstantiableObject::RegisterType("s_bloom", new BloomShader());
	InstantiableObject::RegisterType("s_grass", new GrassShader());
	InstantiableObject::RegisterType("s_animated_model", new AnimatedModelShader());
}

void ShadersLibrary::Load()
{
	AddShader("s_default", new DefaultShader());
	AddShader("s_overdraw", new OverdrawShader());
	AddShader("s_text", new TextShader());
	AddShader("s_shadow", new ShadowShader());
	AddShader("s_gui", new GUIShader());
	AddShader("s_normalmap", new NormalMapShader());
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
