#include "stdafx.h"
#include "ModelNormalMapRenderer.h"
#include "../resources/models/Model.h"
#include "../resources/textures/Texture.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/shaders/NormalMapShader.h"
#include "../resources/entities/Light.h"
#include "../resources/camera/ICamera.h"
#include "../renderer/ModelRenderer.h"
#include <GL/glew.h>

ModelNormalMapRenderer::ModelNormalMapRenderer(Model* model, IShaderProgram* shader, const Texture* texture, const Texture* normalmap, const Light* light) :
ModelRenderer(model, shader, texture, light),
mNormalmapTexture(normalmap)
{
	mTangents = mModel->GetTangents();
}


ModelNormalMapRenderer::~ModelNormalMapRenderer()
{
}

const std::string ModelNormalMapRenderer::GetName() const
{
	return std::string("ModelNormalMapRenderer_")  + std::to_string(mModel->GetID());
}


void ModelNormalMapRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	ModelRenderer::PreRender(vertexBufferManager);

	//4rd tangents
	if (mTangents.size() > 0)
	{
		GLint tangentsModelspaceID = mShaderProgram->GetAttributeLocation("tangentModelspace");
		if (tangentsModelspaceID != -1)
		{
			glGenBuffers(1, &mTangentVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mTangentVBO);
			glBufferData(GL_ARRAY_BUFFER, mTangents.size() * sizeof(glm::vec3), &mTangents[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(tangentsModelspaceID);
			glVertexAttribPointer(
				tangentsModelspaceID,  // The attribute we want to configure
				3,                            // size
				GL_FLOAT,                     // type
				GL_FALSE,                     // normalized?
				0,                            // stride
				(void*)0                      // array buffer offset
			);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	if (mNormalmapTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + mNormalmapTexture->GetUnit());
		glBindTexture(GL_TEXTURE_2D, mNormalmapTexture->GetID());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ModelNormalMapRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	NormalMapShader* shader = static_cast<NormalMapShader*>(mShaderProgram);

	if (mTexture != nullptr)
	{
		shader->LoadModelTexture(mTexture->GetUnit());
	}
	if (mNormalmapTexture != nullptr)
	{
		shader->LoadModelNormalmap(mNormalmapTexture->GetUnit());
	}
	shader->LoadLight(*mLight);
	shader->LoadViewMatrix(const_cast<ICamera*>(camera)->GetViewMatrix());
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadFogParameters(mFogColor, mIsFogEnabled ? mFogDensity : 0.0f, mFogGradient);
	shader->LoadTiling(mTile);
	shader->LoadCameraPosition(camera->GetPosition());
	if (mTextureShadowmap != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + mTextureShadowmap->GetUnit());
		glBindTexture(GL_TEXTURE_2D, mTextureShadowmap->GetID());
		shader->LoadShadowMapSpaceMatrix(mShadowSpaceMatrix);
		shader->LoadShadowMapTexture(mTextureShadowmap->GetUnit(), mTextureShadowmap->GetWidth());
		shader->LoadShadowMapPFC(mPFCCounter);
	}

	std::vector<glm::mat4> matrices;
	int instances = 1;

	if (mIsInstancingEnabled)
	{
		for (IRenderer* renderer : mInstances)
		{
			glm::mat4 modelMatrix = renderer->GetModelMatrix();
			matrices.push_back(modelMatrix);
		}
		instances = mInstances.size();
	}
	else
	{
		glm::mat4 modelMatrix = mParent->GetTransformation()->GetModelMatrix();
		matrices.push_back(modelMatrix);
	}
	if (mMatrixVBO == -1)
	{
		mMatrixVBO = vertexBufferManager.GetVBO("Matrix_" + GetName());
	}
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, &matrices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}