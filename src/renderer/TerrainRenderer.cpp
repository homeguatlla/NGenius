#include "stdafx.h"
#include "TerrainRenderer.h"
#include <GLFW/glfw3.h>
#include <assert.h>

#include <glm/glm.hpp>
#include "../resources/shaders/TerrainShader.h"
#include "../resources/textures/Texture.h"
#include "../resources/textures/TextureArray.h"
#include "../resources/entities/Light.h"
#include "../resources/camera/ICamera.h"

TerrainRenderer::TerrainRenderer(IShaderProgram* shader, const Texture* textureHeightmap, const Texture* textureBlendmap, const TextureArray* textureArray, const Light* light, float scale) :
IRenderer(shader),
mVertexVAO(-1),
mVertexVBO(-1),
mTextureCoordsVBO(-1),
mIndexVBO(-1),
mMatrixID(-1),
mTextureHeightmap(textureHeightmap),
mTextureBlendmap(textureBlendmap),
mTextureArray(textureArray),
mLight(light),
mScale(scale),
mClippingPlane(0.0f, 1.0f, 0.0f, 1000.0f),
mShadowSpaceMatrix(),
mTextureShadowmap(nullptr)
{
	assert(mTextureHeightmap != nullptr);
	assert(mTextureBlendmap != nullptr);
	assert(mTextureArray->GetNumberTextures() == 4);
	mBitRenderInformation.SetTexture(textureHeightmap->GetID());
}


TerrainRenderer::~TerrainRenderer()
{
	if (mIsPrerendered)
	{ 
		glDeleteVertexArrays(1, &mVertexVAO);
		glDeleteBuffers(1, &mVertexVBO);
		glDeleteBuffers(1, &mTextureCoordsVBO);
	}
}

const std::string TerrainRenderer::GetName() const
{
	return "TerrainRenderer";
}


void TerrainRenderer::SetTextureCoords(const std::vector<glm::vec2>& uv)
{
	mTextureCoords = uv;
	mIsPrerendered = false;
}

void TerrainRenderer::SetClippingPlane(const glm::vec4& plane)
{
	mClippingPlane = plane;
}

bool TerrainRenderer::HasClippingPlane() const
{
	return true;
}

void TerrainRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	IRenderer::PreRender(vertexBufferManager);

	// 2nd attribute buffer : texture coords
	GLint textureID = mShaderProgram->GetAttributeLocation("textureCoordsModelspace");
	if (textureID != -1)
	{
		glGenBuffers(1, &mTextureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mTextureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, mTextureCoords.size() * sizeof(glm::vec2), &mTextureCoords[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(textureID);
		glVertexAttribPointer(
			textureID,  // The attribute we want to configure
			2,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
	}
	glActiveTexture(GL_TEXTURE0 + mTextureHeightmap->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mTextureHeightmap->GetID());

	glActiveTexture(GL_TEXTURE0 + mTextureBlendmap->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mTextureBlendmap->GetID());

	glActiveTexture(GL_TEXTURE0 + mTextureArray->GetUnit());
	glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureArray->GetID());

	mMatrixID = mShaderProgram->GetUniformLocation("MVP");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TerrainRenderer::Draw()
{
	IRenderer::Draw();
}

bool TerrainRenderer::IsInstancingAllowed() const
{
	return false;
}

void TerrainRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	TerrainShader* shader = static_cast<TerrainShader*>(mShaderProgram);

	shader->Use();
	shader->LoadScale(mScale);
	shader->LoadHeightMapTexture(mTextureHeightmap->GetUnit());
	shader->LoadBlendMapTexture(mTextureBlendmap->GetUnit());
	shader->LoadArrayTexture(mTextureArray->GetUnit());
	shader->LoadCameraPosition(camera->GetPosition());
	shader->LoadLight(*mLight);
	shader->LoadModelMatrix(mParent->GetTransformation()->GetModelMatrix());
	shader->LoadTile(50.0f);
	shader->LoadFogParameters(mFogColor, mIsFogEnabled ? mFogDensity : 0.0f, mFogGradient);
	shader->LoadClippingPlane(mClippingPlane);
	if (mTextureShadowmap != nullptr)
	{
		shader->LoadShadowMapSpaceMatrix(mShadowSpaceMatrix);
		shader->LoadShadowMapTexture(mTextureShadowmap->GetUnit(), mTextureShadowmap->GetWidth());
		shader->LoadShadowMapPFC(mPFCCounter);
	}

	glm::mat4 MVP = camera->GetProjectionMatrix() * const_cast<ICamera*>(camera)->GetViewMatrix() * mParent->GetTransformation()->GetModelMatrix();

	glUniformMatrix4fv(mMatrixID, 1, GL_FALSE, &MVP[0][0]);
}

int TerrainRenderer::GetRenderShaderPassTextureUnit() const
{
	return mTextureBlendmap->GetUnit();
}

void TerrainRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{	
	IRenderer::Render(camera, vertexBufferManager);
}

bool TerrainRenderer::HasFog() const
{
	return true;
}

void TerrainRenderer::EnableFog(bool enable)
{
	mIsFogEnabled = enable;
}

bool TerrainRenderer::IsCastingShadows() const
{
	return true;
}

void TerrainRenderer::SetShadowMapParameters(const Texture* shadowMap, const glm::mat4& matrix, int pfcCounter)
{
	mTextureShadowmap = shadowMap;
	mShadowSpaceMatrix = matrix;
	mPFCCounter = pfcCounter;
}