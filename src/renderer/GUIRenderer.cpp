#include "stdafx.h"
#include "GUIRenderer.h"
#include "../resources/textures/Texture.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/shaders/QuadShader.h"
#include "../resources/camera/ICamera.h"
#include "../resources/GameEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

GUIRenderer::GUIRenderer(IShaderProgram* shader, const Texture* texture, float width, float height) :
IRenderer(shader),
mTexture(texture),
mWidth(width),
mHeight(height),
mIsBillboard(false),
mTextureCoordsVBO(-1),
mMatrixVBO(-1)
{
	Create();
	assert(texture != nullptr);
	mBitRenderInformation.SetTexture(texture->GetID());
	SetLayer(IRenderer::LAYER_GUI);
}

const std::string GUIRenderer::GetName() const
{
	return "GUIRenderer";
}

GUIRenderer::~GUIRenderer()
{
	if (mIsPrerendered)
	{
		glDeleteBuffers(1, &mTextureCoordsVBO);
		glDeleteBuffers(1, &mMatrixVBO);
	}
	mInstances.clear();
}

GUIRenderer* GUIRenderer::DoClone() const
{
	GUIRenderer* clone = new GUIRenderer(*this);

	return clone;
}

void GUIRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	IRenderer::Render(camera, vertexBufferManager);
}

bool GUIRenderer::IsInstancingAllowed() const
{
	return true;
}

bool GUIRenderer::HasFog() const
{
	return false;
}

bool GUIRenderer::HasClippingPlane() const
{
	return false;
}

void GUIRenderer::SetBillboard(bool isBillboard)
{
	mIsBillboard = isBillboard;
}

void GUIRenderer::Create()
{
	std::vector<glm::vec3> vertexs;
	vertexs.push_back(glm::vec3(-mWidth, mHeight, 0.0f));
	vertexs.push_back(glm::vec3(mWidth, mHeight, 0.0f));
	vertexs.push_back(glm::vec3(mWidth, -mHeight, 0.0f));
	vertexs.push_back(glm::vec3(-mWidth, -mHeight, 0.0f));
	SetVertexs(vertexs);

	mTextureCoords.push_back(glm::vec2(0.0f, 1.0f));
	mTextureCoords.push_back(glm::vec2(1.0f, 1.0f));
	mTextureCoords.push_back(glm::vec2(1.0f, 0.0f));
	mTextureCoords.push_back(glm::vec2(0.0f, 0.0f));

	std::vector<unsigned int> indexs;
	indexs.push_back(0);
	indexs.push_back(3);
	indexs.push_back(2);

	indexs.push_back(0);
	indexs.push_back(2);
	indexs.push_back(1);	
	SetIndexes(indexs);
}

void GUIRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
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

void GUIRenderer::Draw()
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
	
void GUIRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	const glm::mat4 viewMatrix = const_cast<ICamera*>(camera)->GetViewMatrix();
	QuadShader* shader = static_cast<QuadShader*>(mShaderProgram);
	shader->LoadQuadTexture(mTexture->GetUnit());
	shader->LoadViewMatrix(viewMatrix);
	shader->LoadProjectionMatrix(camera->GetProjectionMatrix());

	std::vector<glm::mat4> matrices;
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
	}

	if (mMatrixVBO == -1)
	{
		mMatrixVBO = vertexBufferManager.GetVBO("Matrix_" + GetName());
	}
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, &matrices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int GUIRenderer::GetRenderShaderPassTextureUnit() const
{
	return mTexture->GetUnit();
}

void GUIRenderer::ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix)
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