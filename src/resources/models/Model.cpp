#include "stdafx.h"
#include "Model.h"
#include "Mesh.h"
#include "../materials/IMaterial.h"
#include "../shaders/IShaderProgram.h"

#include <GL/glew.h>

Model::Model(Mesh* model) :
mMesh(model),
mVAO(-1),
mMatrixVBO(-1),
mVertexsVBO(-1),
mTextureCoordsVBO(-1),
mIndexesVBO(-1)
{
	assert(model != nullptr);
	
	if (model->GetNumberOfVertexs() > 0)
	{
		CalculateAABB();
	}
}

Model::~Model()
{
	delete mMesh;
	//TODO falta eliminar el VAO y los VBO que se hayan creado
	//entonces quizá no hace falta que cuando se creen se use el vertexbuffermanager, 
	//o si se usa, hay que guardar una referencia a este para poder hacer el remove vao, y por tanto se tiene que pasar en el constructor
	//y ya no hace falta pasarlo al hacer el build.
	//o, se crea un método destroy que recibe como parámetro el vertex buffer array y se puede eliminar el vao
}

unsigned int Model::GetID() const
{
	return mMesh->GetID();
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
	return mMesh->GetNumberOfVertexs();
}

int Model::GetNumberOfIndexes() const
{
	return mMesh->GetNumberOfIndexes();
}

void Model::Apply(std::vector<glm::mat4>& matrices)
{
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * matrices.size(), &matrices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Model::IsBuilt() const 
{
	return mVAO != -1;
}

void Model::UpdateVBOs()
{
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexsVBO);
	glBufferData(GL_ARRAY_BUFFER, mMesh->GetNumberOfVertexs() * sizeof(glm::vec3), &mMesh->GetVertexs()[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->GetNumberOfIndexes() * sizeof(unsigned int), &mMesh->GetIndexes()[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mTextureCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, mMesh->GetNumberOfTextureCoords() * sizeof(glm::vec2), &mMesh->GetTextureCoords()[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Model::Build(VertexBuffersManager& vertexBufferManager, IMaterial* material)
{
	std::string name("model_");
	name.append(std::to_string(GetID()));

	mVAO = vertexBufferManager.CreateVAO(name);
	glBindVertexArray(mVAO);

	int location = material->GetShader()->GetAttributePosition();
	if ( location != -1)
	{
		CreateVertexsVBO(vertexBufferManager, location);
	}
	//TODO los atributos de location deberían estar en algún lado definidos dentro del Shader o del material
	location = material->GetShader()->GetAttributeLocation("textureCoordsModelspace");
	if (location != -1)
	{
		CreateTextureCoordsVBO(vertexBufferManager, location);
	}

	CreateIndexesVBO(vertexBufferManager);

	location = material->GetShader()->GetAttributeLocation("normalModelspace");
	if (location != -1)
	{
		CreateNormalsVBO(vertexBufferManager, location);
	}

	location = material->GetShader()->GetAttributeLocation("tangentModelspace");
	if (location != -1)
	{
		CreateTangentsVBO(vertexBufferManager, location);
	}

	location = material->GetShader()->GetAttributeLocation("M");
	if (location != -1)
	{
		CreateModelMatrixVBO(vertexBufferManager, location);
	}
	//TODO add normal, texture, tangent, model matrix, color
	glBindVertexArray(0);
}

void Model::CreateModelMatrixVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	//matrices instanced
	std::string name("model_matrix_");
	name.append(std::to_string(GetID()));
	mMatrixVBO = vertexBufferManager.CreateVBO(name);
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVBO);

	for (unsigned int i = 0; i < 4; ++i)
	{
		glEnableVertexAttribArray(location + i);
		glVertexAttribPointer(location + i,
								4, GL_FLOAT, GL_FALSE,
								sizeof(glm::mat4),
								(void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisorARB(location + i, 1);
		//glDisableVertexAttribArray(matrixLocation + i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::CreateIndexesVBO(VertexBuffersManager& vertexBufferManager)
{
	long numIndexes = mMesh->GetNumberOfIndexes();
	if (numIndexes > 0)
	{
		//to index geometry
		std::string name("model_indexes_");
		name.append(std::to_string(GetID()));
		mIndexesVBO = vertexBufferManager.CreateVBO(name);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexesVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), &mMesh->GetIndexes()[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Model::CreateVertexsVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	long numVertexs = mMesh->GetNumberOfVertexs();
	if (numVertexs > 0)
	{
		// 1rst attribute buffer : vertices
		std::string name("model_vertexs_");
		name.append(std::to_string(GetID()));

		mVertexsVBO = vertexBufferManager.CreateVBO(name);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexsVBO);
		glBufferData(GL_ARRAY_BUFFER, numVertexs * sizeof(glm::vec3), &mMesh->GetVertexs()[0], GL_STATIC_DRAW);

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

void Model::CreateTextureCoordsVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	// 2nd attribute buffer : texture coords
	long numTextureCoords = mMesh->GetNumberOfTextureCoords();
	if (numTextureCoords > 0)
	{
		std::string name("model_texture_coords_");
		name.append(std::to_string(GetID()));

		mTextureCoordsVBO = vertexBufferManager.CreateVBO(name);

		glBindBuffer(GL_ARRAY_BUFFER, mTextureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, numTextureCoords * sizeof(glm::vec2), &mMesh->GetTextureCoords()[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(location);
		glVertexAttribPointer(
			location,  // The attribute we want to configure
			2,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Model::CreateNormalsVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	//3rd normals
	long numNormals = mMesh->GetNumberOfNormals();
	if (numNormals > 0)
	{
		std::string name("model_normals_");
		name.append(std::to_string(GetID()));

		unsigned int normalsVBO = vertexBufferManager.CreateVBO(name);
		glGenBuffers(1, &normalsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
		glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(glm::vec3), &mMesh->GetNormals()[0], GL_STATIC_DRAW);

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

void Model::CreateTangentsVBO(VertexBuffersManager& vertexBufferManager, int location)
{
	//4rd tangents
	long numTangents = mMesh->GetNumberOfTangents();
	if (numTangents > 0)
	{
		std::string name("model_tangents_");
		name.append(std::to_string(GetID()));

		unsigned int tangentsVBO = vertexBufferManager.CreateVBO(name);
		glGenBuffers(1, &tangentsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentsVBO);
		glBufferData(GL_ARRAY_BUFFER, numTangents * sizeof(glm::vec3), &mMesh->GetTangents()[0], GL_STATIC_DRAW);

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

void Model::CalculateAABB()
{
	glm::vec3 min(mMesh->GetVertexs()[0]);
	glm::vec3 max(mMesh->GetVertexs()[0]);

	for (glm::vec3 vertex : mMesh->GetVertexs())
	{
	min = glm::min(min, vertex);
	max = glm::max(max, vertex);
	}
	mAABB.SetVertexMinMax(min, max);
}