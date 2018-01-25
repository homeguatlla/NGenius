#include "stdafx.h"
#include "CubeRenderer.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/camera/ICamera.h"
#include "../resources/GameEntity.h"
#include <GL/glew.h>

CubeRenderer::CubeRenderer(IShaderProgram* shader) : 
IRenderer(shader),
mMatrixID(-1)
{
	Create();
}


CubeRenderer::~CubeRenderer()
{
}

const std::string CubeRenderer::GetName() const
{
	return "CubeRenderer";
}

void CubeRenderer::Create()
{
	std::vector<glm::vec3> vertexs;
	vertexs.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertexs.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertexs.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertexs.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertexs.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertexs.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertexs.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertexs.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));

	SetVertexs(vertexs);

	std::vector<unsigned int> indexes;
	indexes.push_back(0);
	indexes.push_back(1);
	indexes.push_back(2);
	indexes.push_back(2);
	indexes.push_back(1);
	indexes.push_back(3);
	indexes.push_back(2);
	indexes.push_back(3);
	indexes.push_back(4);
	indexes.push_back(4);
	indexes.push_back(3);
	indexes.push_back(5);
	indexes.push_back(4);
	indexes.push_back(5);
	indexes.push_back(6);
	indexes.push_back(6);
	indexes.push_back(5);
	indexes.push_back(7);
	indexes.push_back(6);
	indexes.push_back(7);
	indexes.push_back(0);
	indexes.push_back(0);
	indexes.push_back(7);
	indexes.push_back(1);
	indexes.push_back(6);
	indexes.push_back(0);
	indexes.push_back(2);
	indexes.push_back(2);
	indexes.push_back(4);
	indexes.push_back(6);
	indexes.push_back(7);
	indexes.push_back(5);
	indexes.push_back(3);
	indexes.push_back(7);
	indexes.push_back(3);
	indexes.push_back(1);

	SetIndexes(indexes);
}

void CubeRenderer::PreRender(VertexBuffersManager& vertexBuffersManager)
{
	IRenderer::PreRender(vertexBuffersManager);
	mMatrixID = mShaderProgram->GetUniformLocation("MVP");
}

void CubeRenderer::Draw()
{
	glDrawElements(GL_LINES, mIndexes.size(), GL_UNSIGNED_INT, 0);
}

bool CubeRenderer::IsInstancingAllowed() const
{
	return false;
}

bool CubeRenderer::HasFog() const
{
	return false;
}

bool CubeRenderer::HasClippingPlane() const
{
	return false;
}

void CubeRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBuffersManager)
{
	IRenderer::Render(camera, vertexBuffersManager);
}

void CubeRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	glm::mat4 MVP = camera->GetProjectionMatrix() * const_cast<ICamera*>(camera)->GetViewMatrix() * mParent->GetTransformation()->GetModelMatrix();

	glUniformMatrix4fv(mMatrixID, 1, GL_FALSE, &MVP[0][0]);
}

int CubeRenderer::GetRenderShaderPassTextureUnit() const
{
	return -1;
}