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
#include <GL/glew.h>
#include <iostream>

IRenderer_::IRenderer_(Model* model, IMaterial* material) :
mParent(nullptr),
mModel(model),
mIsPrerendered(false),
mIsInstancingEnabled(false),
mLayer(LAYER_OTHER),
mMaterial(material),
mIsVisible(true)
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

	for (IRenderer_* renderer : mInstances)
	{
		glm::mat4 modelMatrix = renderer->GetParent()->GetTransformation()->GetModelMatrix();
		matrices.push_back(modelMatrix);
	}

	mModel->Apply(matrices);
	material->Apply(camera);
		
	Draw();

	glBindVertexArray(0);
}

void IRenderer_::Draw()
{
	if (mIsInstancingEnabled)
	{
		//glDrawElementsInstancedARB(GL_TRIANGLES, mIndexes.size(), GL_UNSIGNED_INT, 0, mInstances.size());
		glDrawArraysInstanced(GL_TRIANGLES, 0, mModel->GetNumberOfVertexs(), mInstances.size());
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, mModel->GetNumberOfVertexs());
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