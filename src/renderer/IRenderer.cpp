#include "stdafx.h"
#include "IRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/camera/ICamera.h"
#include "../resources/GameEntity.h"
#include <GL/glew.h>
#include <iostream>

IRenderer::IRenderer(IShaderProgram* shader) :
mIsPrerendered(false),
mIsInstancingEnabled(false),
mLayer(LAYER_OTHER),
mShaderProgram(shader),
mVAO(-1),
mVertexVBO(-1),
mIndexVBO(-1),
mParent(nullptr),
mFogDensity(0.0f),
mFogGradient(1.0f),
mFogColor(0.0f),
mIsFogEnabled(false),
mIsVisible(true),
mTile(1.0f)
{
	assert(mShaderProgram != nullptr);
	mBitRenderInformation.SetShader(mShaderProgram->GetProgramID());
	mBitRenderInformation.SetLayer(LAYER_OTHER);
	mBitRenderInformation.SetTransparency(false);
}

IRenderer::~IRenderer()
{
	if (mIsPrerendered)
	{
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVertexVBO);
		glDeleteBuffers(1, &mIndexVBO);
	}
}

void IRenderer::SetVertexs(const std::vector<glm::vec3>& vertexs)
{
	mVertexs = vertexs;
	mIsPrerendered = false;
	CalculateBoundingBox();
}

void IRenderer::SetIndexes(const std::vector<unsigned int>& indexes)
{
	mIndexes = indexes;
	mIsPrerendered = false;
}

bool IRenderer::IsPrerendered() const
{
	return mIsPrerendered;
}

void IRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	if (mVertexs.size() > 0 && mIndexes.size() > 0)
	{
		// 1rst attribute buffer : vertices
		mVertexVBO = vertexBufferManager.CreateVBO("vertex_" + GetName());
		glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexs.size() * sizeof(glm::vec3), &mVertexs[0], GL_STATIC_DRAW);
		
		GLuint vertexModelspaceID = mShaderProgram->GetAttributePosition();
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

		//to index geometry
		mIndexVBO = vertexBufferManager.CreateVBO("index_" + GetName());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(unsigned int), &mIndexes[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void IRenderer::SetParent(GameEntity* parent)
{
	mParent = parent;
}

void IRenderer::SetFogParameters(const glm::vec3& color, float density, float gradient)
{
	mFogColor = color;
	mFogDensity = density;
	mFogGradient = gradient;
}

GameEntity* IRenderer::GetParent()
{
	return mParent;
}

void IRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager, IShaderProgram* shaderRenderPass)
{
	if (shaderRenderPass != nullptr)
	{
		std::swap(mShaderProgram, shaderRenderPass);
		mIsPrerendered = false;
	}

	Render(camera, vertexBufferManager);

	if (shaderRenderPass != nullptr)
	{
		std::swap(mShaderProgram, shaderRenderPass);
	}
}

void IRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	if (mVertexs.size() > 0 && mIndexes.size() > 0)
	{
		mShaderProgram->Use();

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
		
		LoadData(camera, vertexBufferManager);
		
		Draw();
		mShaderProgram->UnUse();

		glBindVertexArray(0);
	}
}

void IRenderer::Draw()
{
	glDrawElements(GL_TRIANGLES, mIndexes.size(), GL_UNSIGNED_INT, 0);
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

void IRenderer::SetTile(float tile)
{
	mTile = tile;
}

void IRenderer::EnableInstancing(bool enable)
{
	mIsInstancingEnabled = enable;
}

void IRenderer::SetInstances(std::vector<IRenderer*> instances)
{
	mInstances = instances;
}

glm::mat4 IRenderer::GetModelMatrix()
{
	return mParent->GetTransformation()->GetModelMatrix();
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

const AABB& IRenderer::GetBoundingBox() const
{
	return mBoundingBox;
}

void IRenderer::CalculateBoundingBox()
{
	glm::vec3 min(mVertexs[0]);
	glm::vec3 max(mVertexs[0]);

	for (glm::vec3 vertex : mVertexs)
	{
		min = glm::min(min, vertex);
		max = glm::max(max, vertex);
	}
	mBoundingBox.SetVertexMinMax(min, max);
}