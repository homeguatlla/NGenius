#include "stdafx.h"
#include "EnergyWallRenderer.h"
#include "../resources/models/Model.h"
#include "../resources/textures/Texture.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/shaders/EnergyWallShader.h"
#include "../resources/camera/ICamera.h"
#include "../resources/entities/EnergyWall.h"
#include <GL/glew.h>
#include <iostream>


EnergyWallRenderer::EnergyWallRenderer(Model* model, IShaderProgram* shader, Texture* texture, Texture* depthTexture) :
mModel(model),
IRenderer(shader),
mTexture(texture),
mDepthTexture(depthTexture),
mTextureCoordsVBO(-1),
mNormalVBO(-1),
mMatrixVBO(-1)
{
	//SetIndexes(mModel->GetIndexes());
	SetVertexs(mModel->GetVertexs());
	mTextureCoords = mModel->GetTextureCoords();
	mNormals = mModel->GetNormals();
	mBitRenderInformation.SetModel(model->GetID());
	mBitRenderInformation.SetTexture(texture->GetID());
}


EnergyWallRenderer::~EnergyWallRenderer()
{
	if (IsPrerendered())
	{
		glDeleteBuffers(1, &mTextureCoordsVBO);
		glDeleteBuffers(1, &mMatrixVBO);
	}
	mInstances.clear();
}

const std::string EnergyWallRenderer::GetName() const
{
	return "EnergyWallRenderer";
}

void EnergyWallRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	//TODO: IMPROVEMENT podrías guardar los buffers en un array y hacer una sola llamada a glGenBuffers
	//TODO: IMPROVEMENT en lugar de crear cada vez el array de matrices y pasarlo, puedes usar el glMap, glUnMap functions
	if (mVertexs.size() > 0)
	{
		// 1rst attribute buffer : vertices
		GLint vertexModelspaceID = mShaderProgram->GetAttributePosition();
		if (vertexModelspaceID != -1)
		{
			glGenBuffers(1, &mVertexVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO);
			glBufferData(GL_ARRAY_BUFFER, mVertexs.size() * sizeof(glm::vec3), &mVertexs[0], GL_STATIC_DRAW);

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
			mDepthTexture->SetActive(true);
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
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
			mMatrixVBO = vertexBufferManager.CreateVBO("Matrix_" + GetName());
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
	}
}

void EnergyWallRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	EnergyWallShader* shader = static_cast<EnergyWallShader*>(mShaderProgram);

	shader->LoadModelTexture(mTexture->GetUnit());
	shader->LoadDepthTexture(mDepthTexture->GetUnit());
	shader->LoadViewMatrix(const_cast<ICamera*>(camera)->GetViewMatrix());
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadFogParameters(mFogColor, mIsFogEnabled ? mFogDensity : 0.0f, mFogGradient);
	shader->LoadTile(mTile);
	shader->LoadCameraPosition(camera->GetPosition());
	shader->LoadScreenSize(glm::vec2(mDepthTexture->GetWidth(), mDepthTexture->GetHeight()));

	EnergyWall* energyWall = static_cast<EnergyWall*>(GetParent());
	float time = energyWall->GetLiveTime(); // (particle->GetMaxLiveTime() - particle->GetLiveTime()) / particle->GetMaxLiveTime();
	//std::cout << "time: " << time << "\n";
	shader->LoadTime(time);
	shader->LoadContactPosition(energyWall->GetContactPoint());

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

int EnergyWallRenderer::GetRenderShaderPassTextureUnit() const
{
	return mTexture->GetUnit();
}

void EnergyWallRenderer::Draw()
{
	if (mIsInstancingEnabled)
	{
		//glDrawElementsInstancedARB(GL_TRIANGLES, mIndexes.size(), GL_UNSIGNED_INT, 0, mInstances.size());
		glDrawArraysInstanced(GL_TRIANGLES, 0, mVertexs.size(), mInstances.size());
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, mVertexs.size());
	}
}

bool EnergyWallRenderer::IsInstancingAllowed() const
{
	return false;
}

void EnergyWallRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	if (mVertexs.size() > 0)
	{
		glDisable(GL_CULL_FACE);
		glDepthMask(false);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);// GL_ONE_MINUS_SRC_ALPHA);

		mShaderProgram->Use();

		if (!IsPrerendered())
		{
			// Load it into a VBO
			if (vertexBufferManager.HasVAO(GetName()))
			{
				mVAO = vertexBufferManager.GetVAO(GetName());
				glBindVertexArray(mVAO);
			}
			else
			{
				mVAO = vertexBufferManager.CreateVAO(GetName());
				glBindVertexArray(mVAO);

				PreRender(vertexBufferManager);
			}
			mIsPrerendered = true;
		}
		else
		{
			glBindVertexArray(mVAO);
		}

		LoadData(camera, vertexBufferManager);

		Draw();
		mShaderProgram->UnUse();

		glBindVertexArray(0);

		glDisable(GL_BLEND);
		glDepthMask(true);
		glEnable(GL_CULL_FACE);
	}
}

bool EnergyWallRenderer::HasFog() const
{
	return false;
}

void EnergyWallRenderer::EnableFog(bool enable)
{
	mIsFogEnabled = enable;
}

bool EnergyWallRenderer::HasClippingPlane() const
{
	return false;
}

bool EnergyWallRenderer::IsCastingShadows() const
{
	return false;
}