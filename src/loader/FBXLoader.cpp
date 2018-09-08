#include "stdafx.h"
#include "FBXLoader.h"
#include "glm/glm.hpp"
#include "../resources/models/Mesh.h"
#include "fbxsdk/scene/geometry/fbxlayer.h"

#include <assert.h>
#include <vector>
#include <iostream>

using namespace std;
using namespace fbxsdk;

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

						FbxLayerElementArrayTemplate<FbxVector2>* uvVertices = 0;
						fbxMesh->GetTextureUV(&uvVertices, fbxsdk::FbxLayerElement::eTextureDiffuse);


						int materialsCount = fbxMesh->GetElementMaterialCount();
						for (int m = 0; m < materialsCount; ++m)
						{
							fbxsdk::FbxGeometryElementMaterial* material = fbxMesh->GetElementMaterial(m);
							fbxsdk::FbxLayerElement::EMappingMode mappingMode = material->GetMappingMode();
							
						}
						
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
								bool isMapped = true;
								x = static_cast<float>(uvVertices->GetAt(fbxMesh->GetTextureUVIndex(j, k)).mData[0]);
								y = static_cast<float>(uvVertices->GetAt(fbxMesh->GetTextureUVIndex(j, k)).mData[1]);
								uvs.push_back(glm::vec2(x, y));
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
		int materialsCount = scene->GetMaterialCount();
		if (materialsCount > 0)
		{
			mesh->SetMaterialName(scene->GetMaterial(0)->GetName());
		}
		else 
		{
			std::cout << "	Material not found!\n";
		}

		//load textures
		int numTextures = scene->GetTextureCount();
		for (int textureIndex = 0; textureIndex < numTextures; ++textureIndex)
		{
			FbxTexture* texture = scene->GetTexture(textureIndex);
			FbxTexture::EMappingType type = texture->GetMappingType();
		
			FbxFileTexture* textureFile = static_cast<FbxFileTexture*>(texture);
			if (textureFile != nullptr && !textureFile->GetUserDataPtr())
			{
				const FbxString filename = textureFile->GetFileName();
				std::cout << "	Texture name: " << texture->GetName() << "\n";
				std::cout << "	Texture name: " << filename << "\n";
				if (std::string(texture->GetName()) == "DiffuseTexture")
				{
					mesh->SetDiffuseTextureName(std::string(filename));
				}
				else if (std::string(texture->GetName()) == "NormalTexture")
				{
					mesh->SetNormalMapTextureName(std::string(filename));
				}
				else
				{
					std::cout << "	Texture name not recognized!\n";
				}
			}
		}
	}
}