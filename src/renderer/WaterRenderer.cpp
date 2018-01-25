#include "stdafx.h"
#include "WaterRenderer.h"
#include "../resources/textures/Texture.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/shaders/WaterShader.h"
#include "../resources/camera/ICamera.h"
#include "../resources/GameEntity.h"
#include "../resources/entities/Light.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

WaterRenderer::WaterRenderer(IShaderProgram* shader, const Texture* reflectionTexture, const Texture* refractionTexture, const Texture* distorsion, const Texture* normal, const Texture* depth, float width, float length, float speed, const glm::vec4& waterColor, const Light* light) :
IRenderer(shader),
mReflectionTexture(reflectionTexture),
mRefractionTexture(refractionTexture),
mDistorsionTexture(distorsion),
mNormalTexture(normal),
mDepthTexture(depth),
mWidth(width),
mLength(length),
mWaterSpeed(speed),
mWaterColor(waterColor),
mLight(light)
{
	Create();
	mBitRenderInformation.SetTexture(mReflectionTexture->GetID());
}

WaterRenderer::~WaterRenderer()
{
	if (mIsPrerendered)
	{
		glDeleteBuffers(1, &mTextureCoordsVBO);
		glDeleteBuffers(1, &mMatrixVBO);
	}
	mInstances.clear();
}

const std::string WaterRenderer::GetName() const
{
	return "WaterRenderer";
}

void WaterRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	mCurrentWaterSpeed += mWaterSpeed;
	mCurrentWaterSpeed = glm::mod(mCurrentWaterSpeed, 1.0f);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IRenderer::Render(camera, vertexBufferManager);
	
	//glDisable(GL_BLEND);
}

bool WaterRenderer::IsInstancingAllowed() const
{
	return true;
}

void WaterRenderer::Create()
{
	mVertexs.push_back(glm::vec3(-mWidth, mLength, 0.0f));
	mVertexs.push_back(glm::vec3(mWidth, mLength, 0.0f));
	mVertexs.push_back(glm::vec3(mWidth, -mLength, 0.0f));
	mVertexs.push_back(glm::vec3(-mWidth, -mLength, 0.0f));

	mTextureCoords.push_back(glm::vec2(0.0f, 1.0f));
	mTextureCoords.push_back(glm::vec2(1.0f, 1.0f));
	mTextureCoords.push_back(glm::vec2(1.0f, 0.0f));
	mTextureCoords.push_back(glm::vec2(0.0f, 0.0f));

	mIndexes.push_back(0);
	mIndexes.push_back(3);
	mIndexes.push_back(2);

	mIndexes.push_back(0);
	mIndexes.push_back(2);
	mIndexes.push_back(1);
}

void WaterRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
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
	glActiveTexture(GL_TEXTURE0 + mReflectionTexture->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mReflectionTexture->GetID());

	glActiveTexture(GL_TEXTURE0 + mRefractionTexture->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mRefractionTexture->GetID());

	glActiveTexture(GL_TEXTURE0 + mDistorsionTexture->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mDistorsionTexture->GetID());

	glActiveTexture(GL_TEXTURE0 + mNormalTexture->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mNormalTexture->GetID());

	glActiveTexture(GL_TEXTURE0 + mDepthTexture->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mDepthTexture->GetID());
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//matrices instanced
	GLint matrixLocationID = mShaderProgram->GetAttributeLocation("M");
	if (matrixLocationID != -1)
	{
		mMatrixVBO = vertexBufferManager.CreateVBO("Matrix_" + GetName());

		glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, &matrices[0], GL_DYNAMIC_DRAW);

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

void WaterRenderer::Draw()
{
	if (mIsInstancingEnabled)
	{
		glDrawElementsInstancedARB(GL_TRIANGLES, mIndexes.size(), GL_UNSIGNED_INT, 0, mInstances.size());
	}
	else
	{
		IRenderer::Draw();
	}
}

void WaterRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	WaterShader* shader = static_cast<WaterShader*>(mShaderProgram);
	shader->LoadReflectionTexture(mReflectionTexture->GetUnit());
	shader->LoadRefractionTexture(mRefractionTexture->GetUnit());
	shader->LoadDistorsionTexture(mDistorsionTexture->GetUnit());
	shader->LoadNormalTexture(mNormalTexture->GetUnit());
	shader->LoadDepthTexture(mDepthTexture->GetUnit());
	shader->LoadViewMatrix(const_cast<ICamera*>(camera)->GetViewMatrix());
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadWaterSpeed(mCurrentWaterSpeed);
	shader->LoadWaterColor(mWaterColor);
	shader->LoadCameraPosition(camera->GetPosition());
	shader->LoadLight(mLight);
	shader->LoadFogParameters(mFogColor, mIsFogEnabled ? mFogDensity : 0.0f, mFogGradient);

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

bool WaterRenderer::HasClippingPlane() const
{
	return false;
}

bool WaterRenderer::HasFog() const
{
	return true;
}

void WaterRenderer::EnableFog(bool enable)
{
	mIsFogEnabled = enable;
}