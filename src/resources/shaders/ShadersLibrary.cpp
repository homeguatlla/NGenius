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

ShadersLibrary::ShadersLibrary()
{
}

ShadersLibrary::~ShadersLibrary()
{
}

void ShadersLibrary::Load()
{
	//TODO igual podría haber una fase de init o algo por el estilo.
	AddShader("terrain", new TerrainShader());
	AddShader("default", new DefaultShader());
	AddShader("model", new ModelShader());
	AddShader("normalmap", new NormalMapShader());
	AddShader("skybox", new SkyBoxShader());
	AddShader("particle", new ParticleShader());
	AddShader("gui", new GUIShader());
	AddShader("water", new WaterShader());
	AddShader("skybox", new ParticleShader());
	AddShader("energy_wall", new EnergyWallShader());
	AddShader("text", new TextShader());
	AddShader("shadow", new ShadowShader());
	AddShader("overdraw", new OverdrawShader());
	AddShader("contrast", new ContrastShader());
}

void ShadersLibrary::AddShader(char* name, IShaderProgram* shader)
{
	shader->Init();
	AddElement(name, shader);
}
