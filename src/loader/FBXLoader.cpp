#include "stdafx.h"
#include "FBXLoader.h"
#include "glm/glm.hpp"
#include "../resources/models/Mesh.h"
#include "../resources/models/animation/Joint.h"
#include "../resources/models/animation/Animation.h"
#include "../resources/models/animation/KeyFrame.h"
#include "../resources/models/animation/JointTransform.h"

#include "fbxsdk/scene/geometry/fbxlayer.h"

#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace fbxsdk;

std::map<int, int> FBXLoader::mControlPoints;

FBXLoader::FBXLoader()
{
	
}

FBXLoader::~FBXLoader()
{
}

Mesh* FBXLoader::LoadModel(const std::string& filename, Animation** animation, Joint** rootJoint)
{
	FbxManager* mFbxManager = nullptr;

	if (mFbxManager == nullptr)
	{
		mFbxManager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(mFbxManager, IOSROOT);
		mFbxManager->SetIOSettings(ioSettings);
	}

	FbxImporter* importer = FbxImporter::Create(mFbxManager, "");
	FbxScene* fbxScene = FbxScene::Create(mFbxManager, "");

	std::cout << "Loading Model: " << filename << "\n";

	bool success = importer->Initialize(filename.c_str(), -1, mFbxManager->GetIOSettings());

	if (success)
	{
		success = importer->Import(fbxScene);
		if (success)
		{
			importer->Destroy();
			FbxMesh* fbxMesh = GetMesh(fbxScene);
			Mesh* mesh = LoadMesh(fbxMesh);
			LoadMaterials(fbxScene, mesh);

			std::vector<Joint*> joints;
			*rootJoint = LoadSkeleton(fbxScene, joints);
			*animation = FillAnimationData(fbxScene, fbxMesh, joints, mesh);

			return mesh;
		}
	}
	
	return nullptr;
}

FbxMesh* FBXLoader::GetMesh(FbxScene* fbxScene)
{
	FbxNode* rootNode = fbxScene->GetRootNode();
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
						return fbxMesh;
					}
				}
			}
		}
	}
	return nullptr;
}

Mesh* FBXLoader::LoadMesh(FbxMesh* fbxMesh)
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

			mControlPoints[controlPointIndex] = vertices.size();
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
			if (uvVertices != nullptr)
			{
				x = static_cast<float>(uvVertices->GetAt(fbxMesh->GetTextureUVIndex(j, k)).mData[0]);
				y = static_cast<float>(uvVertices->GetAt(fbxMesh->GetTextureUVIndex(j, k)).mData[1]);
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

	return new Mesh(vertices, uvs, indexes, normals);
}

void FBXLoader::LoadMaterials(FbxScene* fbxScene, Mesh* mesh)
{
	if (fbxScene != nullptr && mesh != nullptr)
	{
		int materialsCount = fbxScene->GetMaterialCount();
		if (materialsCount > 0)
		{
			mesh->SetMaterialName(fbxScene->GetMaterial(0)->GetName());
		}
		else 
		{
			std::cout << "	Material not found!\n";
		}

		//load textures
		int numTextures = fbxScene->GetTextureCount();
		for (int textureIndex = 0; textureIndex < numTextures; ++textureIndex)
		{
			FbxTexture* texture = fbxScene->GetTexture(textureIndex);
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

Joint* FBXLoader::LoadSkeleton(FbxScene* fbxScene, std::vector<Joint*>& joints)
{
	Joint* joint = nullptr;

	FbxNode* fbxNode = fbxScene->GetRootNode();

	if (fbxNode)
	{
		for (int i = 0; i < fbxNode->GetChildCount(); i++)
		{
			FbxNode *node = fbxNode->GetChild(i);
			joint = ProcessSkeletonHeirarchyre(node, joints);
			if (joint != nullptr)
			{
				break;
			}
		}
	}

	return joint;
}

Animation* FBXLoader::FillAnimationData(FbxScene* fbxScene, FbxMesh* fbxMesh, std::vector<Joint*>& joints, Mesh* mesh)
{
	Animation* animation = nullptr;

	int deformerCount = fbxMesh->GetDeformerCount();
	for (int deformer = 0; deformer < deformerCount; ++deformer)
	{
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(fbxMesh->GetDeformer(deformer, FbxDeformer::eSkin));
		if (skin != nullptr)
		{
			for (int clusterIndex = 0; clusterIndex < skin->GetClusterCount(); ++clusterIndex)
			{
				FbxCluster* fbxCluster = skin->GetCluster(clusterIndex);
				std::string jointname = fbxCluster->GetLink()->GetName();
				unsigned int jointIndex = FindJointIndex(jointname, joints);
				if (jointIndex != -1)
				{
					std::cout << "	Joint name:" << jointname << "\n";

					fbxsdk::FbxAMatrix fbxTransformMatrix;
					//FbxAMatrix transformLinkMatrix;
					//FbxAMatrix globalBindposeInverseMatrix;

					fbxCluster->GetTransformMatrix(fbxTransformMatrix);
					//cluster->GetTransformLinkMatrix(transformLinkMatrix);
					glm::mat4x4 transformationMatrix;
					Convert(fbxTransformMatrix, transformationMatrix);
					joints[jointIndex]->SetAnimatedTransform(transformationMatrix);
					//globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;
					for (int i = 0; i < fbxCluster->GetControlPointIndicesCount(); ++i)
					{
						int vertexIndex = FBXLoader::mControlPoints[fbxCluster->GetControlPointIndices()[i]];
						mesh->AddVertexWeightToVertex(vertexIndex, static_cast<float>(fbxCluster->GetControlPointWeights()[i]));
						mesh->AddJointIdToVertex(vertexIndex, jointIndex);
					}
					
					LoadAnimations(fbxScene, fbxCluster, &animation);
				}
				else
				{
					std::cout << "	Joint name:" << jointname << " not found! \n";
				}
			}
		}
	}

	return animation;
}

void FBXLoader::Convert(fbxsdk::FbxAMatrix& matOrigin, glm::mat4& matDestination)
{
	for (int c = 0; c < 4; ++c)
	{
		for (int r = 0; r < 4; ++r)
		{
			matDestination[c][r] = static_cast<float>(matOrigin[c][r]);
		}
	}
}

void FBXLoader::LoadAnimations(FbxScene* fbxScene, FbxCluster* fbxCluster, Animation** animation)
{
	/*int numAnimations = fbxMesh->GetSrcObjectCount();
	for (int animationIndex = 0; animationIndex < numAnimations; animationIndex++)
	{
		FbxAnimStack *animStack = (FbxAnimStack*)fbxMesh->GetSrcObject(animationIndex);
		//FbxAnimEvaluator *animEvaluator = fbxMesh->GetAnimationEvaluator();
		std::cout << "	Animation name:" << animStack->GetName() << "\n";
	}
	
	std::cout << "	Deformer count:" << fbxMesh->GetDeformerCount() << "\n";
	std::cout << "	Shape count:" << fbxMesh->GetShapeCount() << "\n";
	*/
	FbxAnimStack* animStack = fbxScene->GetCurrentAnimationStack();
	if (animStack != nullptr)
	{
		std::string jointname = fbxCluster->GetLink()->GetName();
		FbxTimeSpan time = animStack->GetLocalTimeSpan();
		FbxTime duration = time.GetDuration();

		long long numFrames = duration.GetFrameCount(FbxTime::EMode::eFrames24);
		std::cout << "	Frames count:" << numFrames << "\n";
	
		if ((*animation) == nullptr)
		{
			std::vector<KeyFrame*> keyFrames;
			for (int i = 0; i < numFrames; ++i)
			{
				keyFrames.push_back(new KeyFrame());
			}
			(*animation) = new Animation(std::string(animStack->GetName()), duration.GetMilliSeconds(), keyFrames);
		}

		std::vector<KeyFrame*>& frames = (*animation)->GetKeyframes();

		for (long long f = 0; f < numFrames; ++f)
		{
			assert(frames.size() == numFrames);

			FbxTime time;
			time.SetFrame(f, FbxTime::eFrames30);
			frames[f]->SetTimestamp(time.GetMilliSeconds());

			const fbxsdk::FbxMatrix matrix = fbxCluster->GetLink()->EvaluateGlobalTransform(time);
			fbxsdk::FbxVector4 translation;
			fbxsdk::FbxVector4 rotation;
			fbxsdk::FbxVector4 shearing;
			fbxsdk::FbxVector4 scale;
			double sign = 0.0;

			matrix.GetElements(translation, rotation, shearing, scale, sign);
			glm::quat jointQuaternion(glm::vec3(rotation[0], rotation[1], rotation[2]));
			
			frames[f]->AddJointTransform(jointname, new JointTransform(
														glm::vec3(translation[0], translation[1], translation[2]),
														jointQuaternion));
			
			//FbxAMatrix transformoffset = mesh->GetNode()->EvaluateGlobalTransform(1.0f) * geometryTransform;
			//(*anim)->mGlobalTransform = transformoffset.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);
			
		}
	}
}

unsigned int FBXLoader::FindJointIndex(const std::string & jointname, std::vector<Joint*>& joints)
{
	std::vector<Joint*>::iterator it = std::find_if(joints.begin(), joints.end(), [&](Joint* a) { return a->GetName() == jointname; });

	if (it != joints.end())
	{
		return distance(joints.begin(), it);
	}

	return -1;
}

Joint* FBXLoader::ProcessSkeletonHeirarchyre(FbxNode * fbxNode, std::vector<Joint*>& joints)
{
	Joint* rootJoint = nullptr;
	
	if (fbxNode->GetNodeAttribute() && fbxNode->GetNodeAttribute()->GetAttributeType() &&
		fbxNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		rootJoint = new Joint(joints.size(), std::string(fbxNode->GetName()), glm::mat4(1.0));
		joints.push_back(rootJoint);
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		Joint* newJoint = ProcessSkeletonHeirarchyre(fbxNode->GetChild(i), joints);
		if (newJoint != nullptr)
		{
			if (rootJoint != nullptr)
			{
				rootJoint->AddChild(newJoint);
			}
			else
			{
				rootJoint = newJoint;
			}
		}
	}
	return rootJoint;
}