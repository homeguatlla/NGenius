#include "stdafx.h"
#include "ShadersLibrary.h"
#include "IShaderProgram.h"
#include "DefaultShader.h"
#include "TerrainShader.h"
#include "ModelShader.h"
#include "NormalMapShader.h"
#include "SkyBoxShader.h"
#include "QuadShader.h"
#include "GUIShader.h"
#include "WaterShader.h"
#include "ParticleShader.h"
#include "EnergyWallShader.h"
#include "TextShader.h"
#include "ShadowShader.h"
#include "OverdrawShader.h"

ShadersLibrary::ShadersLibrary()
{
}

ShadersLibrary::~ShadersLibrary()
{
}

void ShadersLibrary::Load()
{
	//TODO igual podría haber una fase de init o algo por el estilo.
	IShaderProgram* program = new TerrainShader();
	program->Init();
	AddElement("terrain", program); 
	
	program = new DefaultShader();
	program->Init();
	AddElement("default", program);

	program = new ModelShader();
	program->Init();
	AddElement("model", program);

	program = new NormalMapShader();
	program->Init();
	AddElement("normalmap", program);

	AddElement("model", program);
	program = new SkyBoxShader();
	program->Init();
	AddElement("skybox", program);

	program = new QuadShader();
	program->Init();
	AddElement("quad", program);

	program = new GUIShader();
	program->Init();
	AddElement("gui", program);

	program = new WaterShader();
	program->Init();
	AddElement("water", program);

	program = new ParticleShader();
	program->Init();
	AddElement("particle", program);

	program = new EnergyWallShader();
	program->Init();
	AddElement("energy_wall", program);

	program = new TextShader();
	program->Init();
	AddElement("text", program);

	program = new ShadowShader();
	program->Init();
	AddElement("shadow", program);

	program = new OverdrawShader();
	program->Init();
	AddElement("overdraw", program);
}
