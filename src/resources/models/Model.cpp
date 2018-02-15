#include "stdafx.h"
#include "Model.h"
#include "ModelGeometry.h"
#include "../materials/IMaterial.h"
#include "../shaders/IShaderProgram.h"

#include <GL/glew.h>

Model::Model(ModelGeometry* model) :
mModelGeometry(model),
mVAO(-1)
{
	assert(model != nullptr);
	assert(model->GetNumberOfVertexs() > 0);
	CalculateAABB();
}

Model::~Model()
{
	delete mModelGeometry;
	//TODO falta eliminar el VAO y los VBO que se hayan creado
	//entonces quizá no hace falta que cuando se creen se use el vertexbuffermanager, 
	//o si se usa, hay que guardar una referencia a este para poder hacer el remove vao, y por tanto se tiene que pasar en el constructor
	//y ya no hace falta pasarlo al hacer el build.
	//o, se crea un método destroy que recibe como parámetro el vertex buffer array y se puede eliminar el vao
}

unsigned int Model::GetID() const
{
	return mModelGeometry->GetID();
}

const AABB& Model::GetAABB() const
{
	return mAABB;
}

int Model::GetVAOID() const
{
	return mVAO;
}

int Model::GetNumberOfVertexs() const
{
	return mModelGeometry->GetNumberOfVertexs();
}

bool Model::IsBuilt() const 
{
	return mVAO != -1;
}

void Model::Build(VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	mVAO = vertexBufferManager.CreateVAO("model_" + mModelGeometry->GetID());
	glBindVertexArray(mVAO);

	int location = material->GetShader()->GetAttributePosition();
	if ( location != -1)
	{
		CreateVertexsBuffer(vertexBufferManager, location);
	}
	//TODO add normal, texture, tangent, model matrix, color
	glBindVertexArray(0);
}

void Model::CreateVertexsBuffer(VertexBuffersManager& vertexBufferManager, int location)
{
	long numVertexs = mModelGeometry->GetNumberOfVertexs();
	if (numVertexs > 0)
	{
		// 1rst attribute buffer : vertices
		if (location != -1)
		{
			unsigned int vertexVBO = vertexBufferManager.CreateVBO("model" + GetID());
			glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
			glBufferData(GL_ARRAY_BUFFER, numVertexs * sizeof(glm::vec3), &mModelGeometry->GetVertexs()[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(
				location,  // The attribute we want to configure
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
	glm::vec3 min(mModelGeometry->GetVertexs()[0]);
	glm::vec3 max(mModelGeometry->GetVertexs()[0]);

	for (glm::vec3 vertex : mModelGeometry->GetVertexs())
	{
	min = glm::min(min, vertex);
	max = glm::max(max, vertex);
	}
	mAABB.SetVertexMinMax(min, max);
}