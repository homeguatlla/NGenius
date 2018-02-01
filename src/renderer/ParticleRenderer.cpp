#include "stdafx.h"
#include "ParticleRenderer.h"
#include "../resources/GameEntity.h"
#include "../resources/entities/Particle.h"
#include "../resources/shaders/ParticleShader.h"
#include "../resources/textures/Texture.h"
#include "../resources/camera/ICamera.h"
#include "../renderer/IRenderer.h"
#include <GL/glew.h>
#include <iostream>

ParticleRenderer::ParticleRenderer(IShaderProgram* shader, const Texture* texture, const Texture* depthTexture, float width, float height) :
QuadRenderer(shader, texture, width, height),
mColorVBO(-1),
mDepthTexture(depthTexture),
mTile(1.0f)
{
	assert(depthTexture != nullptr);
}


ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::SetTile(float tile)
{
	mTile = tile;
}

ParticleRenderer* ParticleRenderer::DoClone() const
{
	//we don't want copies of textures and shaders
	ParticleRenderer* clone = new ParticleRenderer(*this);

	return clone;
}

const std::string ParticleRenderer::GetName() const
{
	return "ParticleRenderer";
}


void ParticleRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	IRenderer::Render(camera, vertexBufferManager);

	glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
}

void ParticleRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
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
		glActiveTexture(GL_TEXTURE0 + mTexture->GetUnit());
		glBindTexture(GL_TEXTURE_2D, mTexture->GetID());
	}

	glActiveTexture(GL_TEXTURE0 + mDepthTexture->GetUnit());
	glBindTexture(GL_TEXTURE_2D, mDepthTexture->GetID());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//color instanced
	GLint colorLocationID = mShaderProgram->GetAttributeLocation("colorGradient");
	if (colorLocationID != -1)
	{
		mColorVBO = vertexBufferManager.CreateVBO("Color_" + GetName());

		glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);

		glEnableVertexAttribArray(colorLocationID);
		glVertexAttribPointer(colorLocationID, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisorARB(colorLocationID, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void ParticleRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	const glm::mat4 viewMatrix = const_cast<ICamera*>(camera)->GetViewMatrix();
	ParticleShader* shader = static_cast<ParticleShader*>(mShaderProgram);
	shader->LoadQuadTexture(mTexture->GetUnit());
	shader->LoadDepthTexture(mDepthTexture->GetUnit());
	shader->LoadViewMatrix(viewMatrix);
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());
	shader->LoadTile(mTile);
	shader->LoadScreenSize(glm::vec2(mDepthTexture->GetWidth(), mDepthTexture->GetHeight()));

	Particle* particle = static_cast<Particle*>(GetParent());
	float time = (particle->GetMaxLiveTime() - particle->GetLiveTime()) / particle->GetMaxLiveTime();
	//std::cout << "time: " << time << "\n";
	shader->LoadTime(time);

	std::vector<glm::mat4> matrices;
	std::vector<glm::vec4> colors;
	int instances = 1;

	if (mIsInstancingEnabled)
	{
		for (IRenderer* renderer : mInstances)
		{
			glm::mat4 modelMatrix = renderer->GetModelMatrix();
			if (mIsBillboard)
			{
				glm::vec3 scale = renderer->GetParent()->GetTransformation()->GetScale();
				float angleZ = renderer->GetParent()->GetTransformation()->GetRotation().z;
				ModifyModelMatrixToAvoidRotations(viewMatrix, scale, angleZ, modelMatrix);
			}
			matrices.push_back(modelMatrix);

			Particle* particle = static_cast<Particle*>(renderer->GetParent());
			const glm::vec4 color = particle->GetColor();
			colors.push_back(color);
		}
		instances = mInstances.size();
	}
	else
	{
		glm::mat4 modelMatrix = mParent->GetTransformation()->GetModelMatrix();

		if (mIsBillboard)
		{
			glm::vec3 scale = mParent->GetTransformation()->GetScale();
			float angleZ = mParent->GetTransformation()->GetRotation().z;
			ModifyModelMatrixToAvoidRotations(viewMatrix, scale, angleZ, modelMatrix);
		}
		matrices.push_back(modelMatrix);

		Particle* particle = static_cast<Particle*>(mParent);
		const glm::vec4 color = particle->GetColor();
		colors.push_back(color);
	}

	if (mMatrixVBO == -1)
	{
		mMatrixVBO = vertexBufferManager.CreateVBO("Matrix_" + GetName());
	}

	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, &matrices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (mColorVBO == -1)
	{
		mColorVBO = vertexBufferManager.CreateVBO("Color_" + GetName());
	}
	glBindBuffer(GL_ARRAY_BUFFER, mColorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instances, &colors[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int ParticleRenderer::GetRenderShaderPassTextureUnit() const
{
	return mTexture->GetUnit();
}