#include "stdafx.h"
#include "NormalRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/camera/ICamera.h"
#include "../resources/GameEntity.h"
#include <GL/glew.h>

NormalRenderer::NormalRenderer(IShaderProgram* shader) : 
IRenderer(shader), 
mMatrixID(-1)
{
}


NormalRenderer::~NormalRenderer()
{
}

const std::string NormalRenderer::GetName() const
{
	return "NormalRenderer";
}

void NormalRenderer::SetNormals(std::vector<glm::vec3>& normals)
{
	mNormals = normals;
}

void NormalRenderer::Create()
{
	/*if (mVertexs.size() > 0 && mNormals.size() == mVertexs.size())
	{
		std::vector<glm::vec3> newVertexs;
		for (unsigned int i = 0; i < mVertexs.size(); ++i)
		{
			glm::vec3 vertex = mVertexs[i];
			glm::vec3 normal = mNormals[i];

			newVertexs.push_back(vertex);
			newVertexs.push_back(vertex + normal * 0.25f);

			mIndexes.push_back(i * 2);
			mIndexes.push_back(i * 2 + 1);
		}
		mVertexs = newVertexs;
		mIsPrerendered = false;
	}*/
}

void NormalRenderer::PreRender(VertexBuffersManager& vertexBufferManager)
{
	IRenderer::PreRender(vertexBufferManager);

	mMatrixID = mShaderProgram->GetUniformLocation("MVP");
}

void NormalRenderer::Draw()
{
	//glDrawElements(GL_LINES, mIndexes.size(), GL_UNSIGNED_INT, 0);
}

void NormalRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	glm::mat4 MVP = camera->GetProjectionMatrix() * const_cast<ICamera*>(camera)->GetViewMatrix() * mParent->GetTransformation()->GetModelMatrix();

	glUniformMatrix4fv(mMatrixID, 1, GL_FALSE, &MVP[0][0]);
}

int NormalRenderer::GetRenderShaderPassTextureUnit() const
{
	return -1;
}

bool NormalRenderer::IsInstancingAllowed() const
{
	return false;
}

bool NormalRenderer::HasFog() const
{
	return false;
}

bool NormalRenderer::HasClippingPlane() const
{
	return false;
}

bool NormalRenderer::IsCastingShadows() const
{
	return false;
}

void NormalRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	IRenderer::Render(camera, vertexBufferManager);
}