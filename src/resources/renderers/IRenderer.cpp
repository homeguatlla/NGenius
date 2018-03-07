#include "stdafx.h"
#include "IRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../shaders/IShaderProgram.h"
#include "../camera/ICamera.h"
#include "../GameEntity.h"
#include "../Transformation.h"
#include "../shaders/QuadShader.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"

//TODO esto hay que quitarlo de aqu� y hacerlo de otra manera
#include "../renderers/ParticleRenderer.h"
#include "../entities/Particle.h"

#include <GL/glew.h>
#include <iostream>
#include <typeinfo.h>

IRenderer::IRenderer(Model* model, IMaterial* material) :
mParent(nullptr),
mModel(model),
mIsPrerendered(false),
mIsInstancingEnabled(false),
mLayer(LAYER_OTHER),
mMaterial(material),
mIsVisible(true),
mIsBillboard(false)
{
	assert(model != nullptr);
	assert(mMaterial != nullptr);
	mBitRenderInformation.SetMaterial(mMaterial->GetMaterialID());
	mBitRenderInformation.SetLayer(LAYER_OTHER);
	mBitRenderInformation.SetModel(model->GetID());
	mBitRenderInformation.SetTransparency(false);
}

IRenderer::~IRenderer()
{
	
}

bool IRenderer::IsPrerendered() const
{
	return mIsPrerendered;
}
/*
void IRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	
}*/

void IRenderer::SetParent(GameEntity* parent)
{
	mParent = parent;
}

GameEntity* IRenderer::GetParent()
{
	return mParent;
}

IMaterial* IRenderer::GetMaterial()
{
	return mMaterial;
}

Model* IRenderer::GetModel()
{
	return mModel;
}

void IRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	glBindVertexArray(mModel->GetVAOID());

	std::vector<glm::mat4> matrices;
	std::vector<glm::vec4> colors;

	const glm::mat4 viewMatrix = const_cast<ICamera*>(camera)->GetViewMatrix();

	//TODO estos bucles son mejorables.
	//la idea es diferenciar entre part�culas y no part�culas.
	if (typeid(*mInstances[0]) == typeid(ParticleRenderer))
	{
		for (IRenderer* renderer : mInstances)
		{
			Transformation* transformation = renderer->GetParent()->GetTransformation();
			
			glm::mat4 modelMatrix = transformation->GetModelMatrix();
			glm::vec3 scale = transformation->GetScale();
			float angleZ = transformation->GetRotation().z;
			ModifyModelMatrixToAvoidRotations(viewMatrix, scale, angleZ, modelMatrix);
			
			matrices.push_back(modelMatrix);

			Particle* particle = static_cast<Particle*>(renderer->GetParent());
			const glm::vec4 color = particle->GetColor();
			colors.push_back(color);
		}
	}
	else
	{
		for (IRenderer* renderer : mInstances)
		{
			Transformation* transformation = renderer->GetParent()->GetTransformation();

			glm::mat4 modelMatrix = transformation->GetModelMatrix();
			if (mIsBillboard)
			{
				glm::vec3 scale = transformation->GetScale();
				float angleZ = transformation->GetRotation().z;
				ModifyModelMatrixToAvoidRotations(viewMatrix, scale, angleZ, modelMatrix);
			}
			matrices.push_back(modelMatrix);
		}
	}

	mModel->Apply(matrices);

	if (!colors.empty())
	{
		mModel->Apply(colors);
	}
	material->Apply(camera, mInstances[0]->GetParent()->GetTransformation());
	
	Draw();

	glBindVertexArray(0);
}

void IRenderer::Draw()
{
	if (mIsInstancingEnabled)
	{
		glDrawElementsInstancedARB(GL_TRIANGLES, mModel->GetNumberOfIndexes(), GL_UNSIGNED_INT, 0, mInstances.size());
		//this doesn't work glDrawArraysInstanced(GL_TRIANGLES, 0, mModel->GetNumberOfIndexes(), mInstances.size());
	}
	else
	{
		glDrawElements(GL_TRIANGLES, mModel->GetNumberOfIndexes(), GL_UNSIGNED_INT, 0);
	}
}

const BitNumber& IRenderer::GetBitRendererInformation() const
{
	return mBitRenderInformation;
}

void IRenderer::SetLayer(char layer)
{
	mLayer = layer;
	mBitRenderInformation.SetLayer(mLayer);
}

char IRenderer::GetLayer() const
{
	return mLayer;
}

void IRenderer::SetTransparency(bool transparent)
{
	mBitRenderInformation.SetTransparency(transparent);
}

void IRenderer::SetDistance(unsigned int distance)
{
	mBitRenderInformation.SetDistance(distance);
}

bool IRenderer::IsVisible() const
{
	return mIsVisible;
}

void IRenderer::SetVisibility(bool visible)
{
	mIsVisible = visible;
}

void IRenderer::EnableInstancing(bool enable)
{
	mIsInstancingEnabled = enable;
}

void IRenderer::SetInstances(std::vector<IRenderer*> instances)
{
	mInstances = instances;
}

void IRenderer::SetBillboard(bool billboard)
{
	mIsBillboard = billboard;
}

IRenderer* IRenderer::Clone() const
{
	IRenderer* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	return clone;
}

void IRenderer::CheckError()
{
	/*
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << err << std::endl;
	}*/
}

const AABB& IRenderer::GetAABB() const
{
	return mModel->GetAABB();
}

void IRenderer::ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix)
{
	modelMatrix[0][0] = viewMatrix[0][0];
	modelMatrix[0][1] = viewMatrix[1][0];
	modelMatrix[0][2] = viewMatrix[2][0];

	modelMatrix[1][0] = viewMatrix[0][1];
	modelMatrix[1][1] = viewMatrix[1][1];
	modelMatrix[1][2] = viewMatrix[2][1];

	modelMatrix[2][0] = viewMatrix[0][2];
	modelMatrix[2][1] = viewMatrix[1][2];
	modelMatrix[2][2] = viewMatrix[2][2];

	modelMatrix = glm::rotate(modelMatrix, angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);
}