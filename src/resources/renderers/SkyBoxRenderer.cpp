#include "stdafx.h"
#include "SkyBoxRenderer.h"
#include "../models/Model.h"
#include "../models/ModelGeometry.h"
#include <GL/glew.h>

SkyBoxRenderer::SkyBoxRenderer(Model* model, IMaterial* material) :
	IRenderer_(model, material)
{
}


SkyBoxRenderer::~SkyBoxRenderer()
{
}

const std::string SkyBoxRenderer::GetName() const
{
	return "SkyBoxRenderer";
}

bool SkyBoxRenderer::IsInstancingAllowed() const
{
	return false;
}

/*
void SkyBoxRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	SkyBoxShader* shader = static_cast<SkyBoxShader*>(mShaderProgram);

	glm::mat4 view = glm::mat4(glm::mat3(const_cast<ICamera*>(camera)->GetViewMatrix()));
	shader->LoadModelMatrix(GetModelMatrix());
	shader->LoadViewMatrix(view);
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadCubemapTexture(mTextureCubemap->GetUnit());
	shader->LoadFogColor(mFogColor);
}*/

void SkyBoxRenderer::Draw()
{
	//draw the skybox last
	//glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	IRenderer_::Draw();

	glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
}