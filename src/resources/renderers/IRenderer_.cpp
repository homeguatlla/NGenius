#include "stdafx.h"
#include "IRenderer_.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../shaders/IShaderProgram.h"
#include "../camera/ICamera.h"
#include "../GameEntity.h"
#include "../shaders/QuadShader.h"
#include "../models/ModelRender.h"
#include "../materials/IMaterial.h"
#include <GL/glew.h>
#include <iostream>

IRenderer_::IRenderer_(ModelRender* model, IMaterial* material) :
mParent(nullptr),
mModel(model),
mIsPrerendered(false),
mIsInstancingEnabled(false),
mLayer(LAYER_OTHER),
mMaterial(material),
mIsVisible(true)
{
	assert(mMaterial != nullptr);
	mBitRenderInformation.SetShader(mMaterial->GetMaterialID());
	mBitRenderInformation.SetLayer(LAYER_OTHER);
	mBitRenderInformation.SetTransparency(false);
}

IRenderer_::~IRenderer_()
{
	
}

bool IRenderer_::IsPrerendered() const
{
	return mIsPrerendered;
}

void IRenderer_::PreRender(VertexBuffersManager& vertexBufferManager)
{
	
}

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
	Render(camera, vertexBufferManager);
}

void IRenderer_::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	/*
	if (!mIsPrerendered)
	{
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
		*/
	LoadData(camera, vertexBufferManager);
		
	Draw();

	glBindVertexArray(0);
}

void IRenderer_::Draw()
{
	//glDrawElements(GL_TRIANGLES, mModel->GetIndexes().size(), GL_UNSIGNED_INT, 0);
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