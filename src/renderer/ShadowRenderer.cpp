#include "stdafx.h"
#include "ShadowRenderer.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/camera/ICamera.h"
#include "../resources/GameEntity.h"
#include <GL/glew.h>

ShadowRenderer::ShadowRenderer(IShaderProgram* shader) :
IRenderer(shader),
mMVPMatrixID(-1)
{
	Create();
}


ShadowRenderer::~ShadowRenderer()
{
}

const std::string ShadowRenderer::GetName() const
{
	return "ShadowRenderer";
}

void ShadowRenderer::Create()
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

void ShadowRenderer::PreRender(VertexBuffersManager& vertexBuffersManager)
{
	IRenderer::PreRender(vertexBuffersManager);
	mMVPMatrixID = mShaderProgram->GetUniformLocation("MVP");
}

void ShadowRenderer::Draw()
{
	glDrawElements(GL_LINES, mIndexes.size(), GL_UNSIGNED_INT, 0);
}

bool ShadowRenderer::IsInstancingAllowed() const
{
	return false;
}

bool ShadowRenderer::HasFog() const
{
	return false;
}

bool ShadowRenderer::HasClippingPlane() const
{
	return false;
}

void ShadowRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBuffersManager)
{
	IRenderer::Render(camera, vertexBuffersManager);
}

void ShadowRenderer::LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	glm::mat4 MVP = camera->GetProjectionMatrix() * const_cast<ICamera*>(camera)->GetViewMatrix() * mParent->GetTransformation()->GetModelMatrix();

	glUniformMatrix4fv(mMVPMatrixID, 1, GL_FALSE, &MVP[0][0]);
}

int ShadowRenderer::GetRenderShaderPassTextureUnit() const
{
	return -1;
}