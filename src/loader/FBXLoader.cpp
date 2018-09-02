#include "stdafx.h"
#include "FBXLoader.h"
#include "glm/glm.hpp"
#include "../resources/models/Mesh.h"

#include <assert.h>
#include <vector>
#include <iostream>

using namespace std;

FBXLoader::FBXLoader()
{
	
}

FBXLoader::~FBXLoader()
{
}

Mesh* FBXLoader::LoadModel(const std::string& filename)
{
	FbxManager* mFbxManager = nullptr;

	if (mFbxManager == nullptr)
	{
		mFbxManager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(mFbxManager, IOSROOT);
		mFbxManager->SetIOSettings(ioSettings);
	}

	FbxImporter* importer = FbxImporter::Create(mFbxManager, "");
	FbxScene* scene = FbxScene::Create(mFbxManager, "");

	std::cout << "Loading Model: " << filename << "\n";

	bool success = importer->Initialize(filename.c_str(), -1, mFbxManager->GetIOSettings());

	if (success)
	{
		success = importer->Import(scene);
		if (success)
		{
			importer->Destroy();
			return LoadMesh(scene);
		}
	}
	
	return nullptr;
}

Mesh* FBXLoader::LoadMesh(FbxScene* scene)
{
	FbxNode* rootNode = scene->GetRootNode();
	if (rootNode != nullptr)
	{
		for (int i = 0; i < rootNode->GetChildCount(); ++i)
		{
			FbxNode* childNode = rootNode->GetChild(i);
			if (childNode != nullptr && childNode->GetNodeAttribute() != nullptr)
			{
				FbxNodeAttribute::EType attributeType = childNode->GetNodeAttribute()->GetAttributeType();
				if (attributeType == FbxNodeAttribute::eMesh)
				{
					FbxMesh* fbxMesh = static_cast<FbxMesh*>(childNode->GetNodeAttribute());
					if (fbxMesh != nullptr)
					{
						std::vector<glm::vec3> normals;
						std::vector<glm::vec3> vertices;
						std::vector<glm::vec2> uvs;
						std::vector<unsigned int> indexes;

						FbxVector4* fbxVertices = fbxMesh->GetControlPoints();
						for (int j = 0; j < fbxMesh->GetPolygonCount(); ++j)
						{
							int numVertices = fbxMesh->GetPolygonSize(j);
							assert(numVertices == 3);
							for (int k = 0; k < numVertices; ++k)
							{
								int controlPointIndex = fbxMesh->GetPolygonVertex(j, k);

								indexes.push_back(controlPointIndex);

								//Getting vertex information
								float x = static_cast<float>(fbxVertices[controlPointIndex].mData[0]);
								float y = static_cast<float>(fbxVertices[controlPointIndex].mData[1]);
								float z = static_cast<float>(fbxVertices[controlPointIndex].mData[2]);

								vertices.push_back(glm::vec3(x, y, z));

								//Getting normal information
								FbxVector4 fbxNorm(0, 0, 0, 0);
								bool success = fbxMesh->GetPolygonVertexNormal(j, k, fbxNorm);
								if (success)
								{
									x = static_cast<float>(fbxNorm[0]);
									y = static_cast<float>(fbxNorm[1]);
									z = static_cast<float>(fbxNorm[2]);

									normals.push_back(glm::vec3(x, y, z));
								}
								
								//Getting texture information
								FbxVector2 fbxUV(0, 0);
								bool isMapped = true;
								success = fbxMesh->GetPolygonVertexUV(j, k, "UVW", fbxUV, isMapped);
								if (success)
								{
									x = static_cast<float>(fbxUV[0]);
									y = static_cast<float>(fbxUV[1]);
									uvs.push_back(glm::vec2(x, y));
								}
							}
						}

						if (indexes.size() <= 0)
						{
							std::cout << "	Indexes not loaded!" << "\n";
						}
						
						if (vertices.size() <= 0)
						{
							std::cout << "	Vertices not loaded!" << "\n";
						}

						if (normals.size() <= 0)
						{
							std::cout << "	Normals not loaded!" << "\n";
						}

						if (uvs.size() <= 0)
						{
							std::cout << "	Texture coords not loaded!" << "\n";
						}

						Mesh* mesh = new Mesh(vertices, uvs, indexes, normals);
						LoadMaterials(scene, mesh);

						return mesh;
					}
				}
			}
		}
	}
	return nullptr;
}

void FBXLoader::LoadMaterials(FbxScene* scene, Mesh* mesh)
{
	if (scene != nullptr && mesh != nullptr)
	{
		int materialCount = scene->GetMaterialCount();
		int numTextures = scene->GetTextureCount();
		for (int textureIndex = 0; textureIndex < numTextures; ++textureIndex)
		{
			FbxTexture* texture = scene->GetTexture(textureIndex);
			FbxFileTexture* textureFile = static_cast<FbxFileTexture*>(texture);
			if (textureFile != nullptr && !textureFile->GetUserDataPtr())
			{
				const FbxString filename = textureFile->GetFileName();
				std::cout << "	Texture name: " << filename << "\n";
				mesh->SetDiffuseTextureName(std::string(filename));
			}
		}
	}
}