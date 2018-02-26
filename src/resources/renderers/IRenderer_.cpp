#include "stdafx.h"
#include "IRenderer_.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../shaders/IShaderProgram.h"
#include "../camera/ICamera.h"
#include "../GameEntity.h"
#include "../Transformation.h"
#include "../shaders/QuadShader.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"

//TODO esto hay que quitarlo de aquí y hacerlo de otra manera
#include "../renderers/ParticleRenderer.h"
#include "../entities/Particle.h"

#include <GL/glew.h>
#include <iostream>
#include <typeinfo.h>

IRenderer_::IRenderer_(Model* model, IMaterial* material) :
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

IRenderer_::~IRenderer_()
{
	
}

bool IRenderer_::IsPrerendered() const
{
	return mIsPrerendered;
}
/*
void IRenderer_::PreRender(VertexBuffersManager& vertexBufferManager)
{
	
}*/

void IRenderer_::SetParent(GameEntity* parent)
{
	mParent = parent;
}

GameEntity* IRenderer_::GetParent()
{
	return mParent;
}

IMaterial* IRenderer_::GetMaterial()
{
	return mMaterial;
}

void IRenderer_::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	if (!mModel->IsBuilt())
	{
		mModel->Build(vertexBufferManager, material);
	}

	glBindVertexArray(mModel->GetVAOID());

	std::vector<glm::mat4> matrices;
	std::vector<glm::vec4> colors;

	const glm::mat4 viewMatrix = const_cast<ICamera*>(camera)->GetViewMatrix();

	for (IRenderer_* renderer : mInstances)
	{
		glm::mat4 modelMatrix = renderer->GetParent()->GetTransformation()->GetModelMatrix();

		if (mIsBillboard)
		{
			glm::vec3 scale = renderer->GetParent()->GetTransformation()->GetScale();
			float angleZ = renderer->GetParent()->GetTransformation()->GetRotation().z;
			ModifyModelMatrixToAvoidRotations(viewMatrix, scale, angleZ, modelMatrix);
		}

		matrices.push_back(modelMatrix);

		if (typeid(*renderer) == typeid(ParticleRenderer))
		{
			Particle* particle = static_cast<Particle*>(mParent);
			const glm::vec4 color = particle->GetColor();
			colors.push_back(color);
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

void IRenderer_::Draw()
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

const BitNumber& IRenderer_::GetBitRendererInformation() const
{
	return mBitRenderInformation;
}

void IRenderer_::SetLayer(char layer)
{
	mLayer = layer;
	mBitRenderInformation.SetLayer(mLayer);
}

char IRenderer_::GetLayer() const
{
	return mLayer;
}

void IRenderer_::SetTransparency(bool transparent)
{
	mBitRenderInformation.SetTransparency(transparent);
}

void IRenderer_::SetDistance(unsigned int distance)
{
	mBitRenderInformation.SetDistance(distance);
}

bool IRenderer_::IsVisible() const
{
	return mIsVisible;
}

void IRenderer_::SetVisibility(bool visible)
{
	mIsVisible = visible;
}

void IRenderer_::EnableInstancing(bool enable)
{
	mIsInstancingEnabled = enable;
}

void IRenderer_::SetInstances(std::vector<IRenderer_*> instances)
{
	mInstances = instances;
}

void IRenderer_::SetBillboard(bool billboard)
{
	mIsBillboard = billboard;
}

IRenderer_* IRenderer_::Clone() const
{
	IRenderer_* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	return clone;
}

void IRenderer_::CheckError()
{
	/*
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << err << std::endl;
	}*/
}

const AABB& IRenderer_::GetAABB() const
{
	return mModel->GetAABB();
}

void IRenderer_::ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix)
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