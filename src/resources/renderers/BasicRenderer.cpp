#include "stdafx.h"
#include "BasicRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../camera/ICamera.h"
#include <GL/glew.h>


BasicRenderer::BasicRenderer(Model* model, IMaterial* material) :
IRenderer_(model, material)
{
}


BasicRenderer::~BasicRenderer()
{
}

const std::string BasicRenderer::GetName() const
{
	return std::string("BasicRenderer") + std::to_string(mModel->GetID());
}

/*
void BasicRenderer::PreRender(VertexBuffersManager& vertexBuffersManager)
{
	//TODO: IMPROVEMENT podrías guardar los buffers en un array y hacer una sola llamada a glGenBuffers
	//TODO: IMPROVEMENT en lugar de crear cada vez el array de matrices y pasarlo, puedes usar el glMap, glUnMap functions
	if (GetNumberOfVertexs() > 0)
	{
		// 1rst attribute buffer : vertices
		GLint vertexModelspaceID = mShaderProgram->GetAttributePosition();
		if (vertexModelspaceID != -1)
		{
			glGenBuffers(1, &mVertexVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO);
			glBufferData(GL_ARRAY_BUFFER, GetNumberOfVertexs() * sizeof(glm::vec3), &mVertexs[0], GL_STATIC_DRAW);

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

		// 2nd attribute buffer : texture coords
		if (mTexture != nullptr)
		{
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
				mTexture->SetActive(true);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		//3rd normals
		if (mNormals.size() > 0)
		{
			GLint normalsModelspaceID = mShaderProgram->GetAttributeLocation("normalModelspace");
			if (normalsModelspaceID != -1)
			{
				glGenBuffers(1, &mNormalVBO);
				glBindBuffer(GL_ARRAY_BUFFER, mNormalVBO);
				glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

				glEnableVertexAttribArray(normalsModelspaceID);
				glVertexAttribPointer(
					normalsModelspaceID,  // The attribute we want to configure
					3,                            // size
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
				);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}

		//matrices instanced
		GLint matrixLocationID = mShaderProgram->GetAttributeLocation("M");
		if (matrixLocationID != -1)
		{
			mMatrixVBO = vertexBuffersManager.CreateVBO("Matrix_" + GetName());
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
}*/

bool BasicRenderer::IsInstancingAllowed() const
{
	return true;
}