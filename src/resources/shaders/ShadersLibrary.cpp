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

const char* ShadersLibrary::SHADOW_SHADER_NAME = "ShadowShader";
const char* ShadersLibrary::DEFAULT_SHADER_NAME = "DefaultShader";
const char* ShadersLibrary::OVERDRAW_SHADER_NAME = "OverdrawShader";
const char* ShadersLibrary::TEXT_SHADER_NAME = "TextShader";
const char* ShadersLibrary::GUI_SHADER_NAME = "GUIShader";
const char* ShadersLibrary::NORMAL_MAP_SHADER_NAME = "NormalMapShader";
const char* ShadersLibrary::CONTRAST_SHADER_NAME = "ContrastShader";


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
	AddShader(ShadersLibrary::DEFAULT_SHADER_NAME, DBG_NEW DefaultShader());
	AddShader(ShadersLibrary::OVERDRAW_SHADER_NAME, DBG_NEW OverdrawShader());
	AddShader(ShadersLibrary::TEXT_SHADER_NAME, DBG_NEW TextShader());
	AddShader(ShadersLibrary::SHADOW_SHADER_NAME, DBG_NEW ShadowShader());
	AddShader(ShadersLibrary::GUI_SHADER_NAME, DBG_NEW GUIShader());
	AddShader(ShadersLibrary::NORMAL_MAP_SHADER_NAME, DBG_NEW NormalMapShader());
	AddShader(ShadersLibrary::CONTRAST_SHADER_NAME, DBG_NEW ContrastShader());
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
