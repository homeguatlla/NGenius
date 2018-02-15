#include "stdafx.h"
#include "Model.h"
#include "ModelGeometry.h"
#include "../materials/IMaterial.h"
#include "../shaders/IShaderProgram.h"

#include <GL/glew.h>

Model::Model(ModelGeometry* model) :
mModel(model)
{
	assert(model != nullptr);
	assert(model->GetNumberVertexs() > 0);
	CalculateAABB();
}

Model::~Model()
{
	delete mModel;
}

unsigned int Model::GetID() const
{
	return mModel->GetID();
}

const AABB& Model::GetAABB() const
{
	return mAABB;
}

void Model::Build(VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	CreateVertexsBuffer(vertexBufferManager, material);
}

void Model::CreateVertexsBuffer(VertexBuffersManager& vertexBufferManager, IMaterial* material)
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

void Model::CalculateAABB()
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