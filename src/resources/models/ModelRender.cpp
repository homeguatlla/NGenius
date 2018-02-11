#include "stdafx.h"
#include "ModelRender.h"
#include "Model.h"
#include "../materials/IMaterial.h"
#include "../shaders/IShaderProgram.h"

#include <GL/glew.h>

ModelRender::ModelRender(Model* model) :
	mModel(model)
{
	assert(model != nullptr);
	CalculateAABB();
}

ModelRender::~ModelRender()
{
}

unsigned int ModelRender::GetID() const
{
	return mModel->GetID();
}

const AABB& ModelRender::GetAABB() const
{
	return mAABB;
}

void ModelRender::Build(VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	CreateVertexsBuffer(vertexBufferManager, material);
}

void ModelRender::CreateVertexsBuffer(VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	long numVertexs = mModel->GetNumberVertexs();
	if (numVertexs > 0)
	{
		// 1rst attribute buffer : vertices
		GLint vertexModelspaceID = material->GetShader()->GetAttributePosition();
		if (vertexModelspaceID != -1)
		{
			unsigned int vertexVBO = vertexBufferManager.CreateVBO("model" + GetID());
			glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
			glBufferData(GL_ARRAY_BUFFER, numVertexs * sizeof(glm::vec3), &mModel->GetVertexs()[0], GL_STATIC_DRAW);

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
		}
	}
}

void ModelRender::CalculateAABB()
{
	glm::vec3 min(mModel->GetVertexs()[0]);
	glm::vec3 max(mModel->GetVertexs()[0]);

	for (glm::vec3 vertex : mModel->GetVertexs())
	{
	min = glm::min(min, vertex);
	max = glm::max(max, vertex);
	}
	mAABB.SetVertexMinMax(min, max);
}