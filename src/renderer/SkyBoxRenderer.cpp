#include "stdafx.h"
#include "SkyBoxRenderer.h"

#include "../resources/textures/TextureCubemap.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/shaders/SkyBoxShader.h"
#include "../resources/camera/ICamera.h"
#include "../resources/GameEntity.h"
#include <GL/glew.h>

SkyBoxRenderer::SkyBoxRenderer(IShaderProgram* shader, TextureCubemap* cubemap, float size) : 
IRenderer(shader), 
mTextureCubemap(cubemap),
mSize(size),
mFogColor(0.0f)
{
	Create();
	mBitRenderInformation.SetTexture(cubemap->GetID());
}


SkyBoxRenderer::~SkyBoxRenderer()
{
}

const std::string SkyBoxRenderer::GetName() const
{
	return "SkyBoxRenderer";
}

void SkyBoxRenderer::Create()
{
	mVertexs.push_back(glm::vec3(-mSize, -mSize, -mSize));
	mVertexs.push_back(glm::vec3(-mSize, mSize, -mSize));
	mVertexs.push_back(glm::vec3(mSize, -mSize, -mSize));
	mVertexs.push_back(glm::vec3(mSize, mSize, -mSize));
	mVertexs.push_back(glm::vec3(mSize, -mSize, mSize));
	mVertexs.push_back(glm::vec3(mSize, mSize, mSize));
	mVertexs.push_back(glm::vec3(-mSize, -mSize, mSize));
	mVertexs.push_back(glm::vec3(-mSize, mSize, mSize));


	mIndexes.push_back(2);
	mIndexes.push_back(1);
	mIndexes.push_back(0);
	
	mIndexes.push_back(3);
	mIndexes.push_back(1);
	mIndexes.push_back(2);

	mIndexes.push_back(4);
	mIndexes.push_back(3);
	mIndexes.push_back(2);
	
	mIndexes.push_back(5);
	mIndexes.push_back(3);
	mIndexes.push_back(4);
	
	mIndexes.push_back(6);
	mIndexes.push_back(5);
	mIndexes.push_back(4);
	
	mIndexes.push_back(7);
	mIndexes.push_back(5);
	mIndexes.push_back(6);
	
	mIndexes.push_back(0);
	mIndexes.push_back(7);
	mIndexes.push_back(6);
	
	mIndexes.push_back(1);
	mIndexes.push_back(7);
	mIndexes.push_back(0);
	
	mIndexes.push_back(2);
	mIndexes.push_back(0);
	mIndexes.push_back(6);
	
	mIndexes.push_back(6);
	mIndexes.push_back(4);
	mIndexes.push_back(2);
	
	mIndexes.push_back(3);
	mIndexes.push_back(5);
	mIndexes.push_back(7);

	mIndexes.push_back(1);
	mIndexes.push_back(3);
	mIndexes.push_back(7);
}

void SkyBoxRenderer::SetFogColor(const glm::vec3& fogColor)
{
	mFogColor = fogColor;
}

void SkyBoxRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	IRenderer::PreRender(vertexBufferManager);
	mTextureCubemap->SetActive(true);
}

bool SkyBoxRenderer::IsInstancingAllowed() const
{
	return false;
}

bool SkyBoxRenderer::HasFog() const
{
	return false;
}

bool SkyBoxRenderer::HasClippingPlane() const
{
	return false;
}

bool SkyBoxRenderer::IsCastingShadows() const
{
	return false;
}

void SkyBoxRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	IRenderer::Render(camera, vertexBufferManager);
}

void SkyBoxRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	SkyBoxShader* shader = static_cast<SkyBoxShader*>(mShaderProgram);

	glm::mat4 view = glm::mat4(glm::mat3(const_cast<ICamera*>(camera)->GetViewMatrix()));
	shader->LoadModelMatrix(GetModelMatrix());
	shader->LoadViewMatrix(view);
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadCubemapTexture(mTextureCubemap->GetUnit());
	shader->LoadFogColor(mFogColor);
}

int SkyBoxRenderer::GetRenderShaderPassTextureUnit() const
{
	return mTextureCubemap->GetUnit();
}

void SkyBoxRenderer::Draw()
{
	//draw the skybox last
	//glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	IRenderer::Draw();

	glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
}