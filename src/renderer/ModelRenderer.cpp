#include "stdafx.h"
#include "ModelRenderer.h"
#include "../resources/models/ModelGeometry.h"
#include "../resources/textures/Texture.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/shaders/ModelShader.h"
#include "../resources/entities/Light.h"
#include "../resources/camera/ICamera.h"
#include <GL/glew.h>

ModelRenderer::ModelRenderer(ModelGeometry* model, IShaderProgram* shader, Texture* texture, const Light* light) :
mModel(model),
IRenderer(shader),
mTexture(texture),
mLight(light),
mTextureCoordsVBO(-1),
mNormalVBO(-1),
mMatrixVBO(-1),
mShadowSpaceMatrix(),
mTextureShadowmap(nullptr)
{
	assert(model != nullptr);
	assert(texture != nullptr);

	//SetIndexes(mModel->GetIndexes());
	mTextureCoords = mModel->GetTextureCoords();
	mNormals = mModel->GetNormals();
	mBitRenderInformation.SetModel(model->GetID());
	if (texture != nullptr)
	{
		mBitRenderInformation.SetTexture(texture->GetID());
	}
}


ModelRenderer::~ModelRenderer()
{
	if (IsPrerendered())
	{
		glDeleteBuffers(1, &mTextureCoordsVBO);
		glDeleteBuffers(1, &mMatrixVBO);
	}
	mInstances.clear();
}

const std::string ModelRenderer::GetName() const
{
	return std::string("ModelRenderer_") + std::to_string(mModel->GetID());
}

void ModelRenderer::PreRender(VertexBuffersManager& vertexBuffersManager)
{
	//TODO: IMPROVEMENT podrías guardar los buffers en un array y hacer una sola llamada a glGenBuffers
	//TODO: IMPROVEMENT en lugar de crear cada vez el array de matrices y pasarlo, puedes usar el glMap, glUnMap functions
	/*if (GetNumberOfVertexs() > 0)
	{
		// 1rst attribute buffer : vertices
		GLint vertexModelspaceID = mShaderProgram->GetAttributePosition();
		if (vertexModelspaceID != -1)
		{
			glGenBuffers(1, &mVertexVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO);
			glBufferData(GL_ARRAY_BUFFER, GetNumberOfVertexs() * sizeof(glm::vec3), &mVertexs[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(vertexModelspaceID);
			glVertexAttribPointer(
				vertexModelspaceID,  // The attribute we want to configure
				3,                            // size
				GL_FLOAT,                     // type
				GL_FALSE,                     // normalized?
				0,                            // stride
				(void*)0                      // array buffer offset
			);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// 2nd attribute buffer : texture coords
		if (mTexture != nullptr)
		{
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
				mTexture->SetActive(true);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		//3rd normals
		if (mNormals.size() > 0)
		{
			GLint normalsModelspaceID = mShaderProgram->GetAttributeLocation("normalModelspace");
			if (normalsModelspaceID != -1)
			{
				glGenBuffers(1, &mNormalVBO);
				glBindBuffer(GL_ARRAY_BUFFER, mNormalVBO);
				glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

				glEnableVertexAttribArray(normalsModelspaceID);
				glVertexAttribPointer(
					normalsModelspaceID,  // The attribute we want to configure
					3,                            // size
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
				);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}

		//matrices instanced
		GLint matrixLocationID = mShaderProgram->GetAttributeLocation("M");
		if (matrixLocationID != -1)
		{
			mMatrixVBO = vertexBuffersManager.CreateVBO("Matrix_" + GetName());
			glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);


			for (unsigned int i = 0; i < 4; ++i)
			{
				glEnableVertexAttribArray(matrixLocationID + i);
				glVertexAttribPointer(matrixLocationID + i,
					4, GL_FLOAT, GL_FALSE,
					sizeof(glm::mat4),
					(void*)(sizeof(glm::vec4) * i));
				glVertexAttribDivisorARB(matrixLocationID + i, 1);
				//glDisableVertexAttribArray(matrixLocation + i);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}*/
}

void ModelRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBuffersManager)
{
	ModelShader* shader = static_cast<ModelShader*>(mShaderProgram);

	if (mTexture != nullptr)
	{
		//shader->LoadModelTexture(mTexture->GetUnit());
	}
	//shader->LoadLight(*mLight);
	//shader->LoadViewMatrix(const_cast<ICamera*>(camera)->GetViewMatrix());
	//shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadFogParameters(mFogColor, mIsFogEnabled ? mFogDensity : 0.0f, mFogGradient);
	//shader->LoadTile(mTile);
	//shader->LoadCameraPosition(camera->GetPosition());
	if (mTextureShadowmap != nullptr)
	{
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
		mMatrixVBO = vertexBuffersManager.GetVBO("Matrix_" + GetName());
	}
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, &matrices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int ModelRenderer::GetRenderShaderPassTextureUnit() const
{
	return mTexture->GetUnit();
}

void ModelRenderer::Draw()
{
	/*if (mIsInstancingEnabled)
	{
		//glDrawElementsInstancedARB(GL_TRIANGLES, mIndexes.size(), GL_UNSIGNED_INT, 0, mInstances.size());
		glDrawArraysInstanced(GL_TRIANGLES, 0, mVertexs.size(), mInstances.size());
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, mVertexs.size());
	}*/
}

bool ModelRenderer::IsInstancingAllowed() const
{
	return true;
}

void ModelRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBuffersManager)
{
	IRenderer::Render(camera, vertexBuffersManager);
}

bool ModelRenderer::HasFog() const
{
	return true;
}

void ModelRenderer::EnableFog(bool enable)
{
	mIsFogEnabled = enable;
}

bool ModelRenderer::HasClippingPlane() const
{
	return false;
}

bool ModelRenderer::IsCastingShadows() const
{
	return true;
}

void ModelRenderer::SetShadowMapParameters(const Texture* shadowMap, const glm::mat4& matrix, int pfcCounter)
{
	mTextureShadowmap = shadowMap;
	mShadowSpaceMatrix = matrix;
	mPFCCounter = pfcCounter;
}