#include "stdafx.h"
#include "AssimpLoader.h"
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>

#include "glm/gtx/transform.hpp"

#include "../resources/models/Mesh.h"
#include "../resources/models/animation/Animation.h"
#include "../resources/models/animation/Joint.h"
#include "../resources/models/animation/KeyFrame.h"
#include "../resources/models/animation/JointTransform.h"

#include "../utils/Log.h"

#include <iostream>

glm::mat4x4 AssimpLoader::CORRECTION_MATRIX = glm::rotate(glm::mat4x4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

AssimpLoader::AssimpLoader()
{
}

AssimpLoader::~AssimpLoader()
{
}

Mesh* AssimpLoader::LoadModel(const std::string& filename, Animation** animation, Joint** rootJoint)
{
	aiPropertyStore* store = aiCreatePropertyStore();
	aiSetImportPropertyInteger(store, AI_CONFIG_IMPORT_COLLADA_IGNORE_UP_DIRECTION, 0);
	const aiScene* assimpScene = aiImportFile(filename.c_str(),	aiProcess_Triangulate |
																aiProcess_JoinIdenticalVertices |
																aiProcess_GenNormals |
																aiProcess_SortByPType |
																aiProcess_FlipUVs |
																aiProcess_OptimizeMeshes
	);

	Mesh* mesh = nullptr;

	if (assimpScene != nullptr && !(assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		aiNode* rootNode = assimpScene->mRootNode;
		std::string rootBoneName;

		if (assimpScene->HasMeshes())
		{
			std::map<std::string, int> jointsIndexesMap;
			std::map<std::string, int> bonesNames;

			ReadAllBonesNames(assimpScene->mMeshes, assimpScene->mNumMeshes, bonesNames);
			//TODO si hay un hueso con más de un nombre podemos sacar un error
			if (rootNode != nullptr)
			{
				bool found = FindFirstSkeletonBoneName(rootNode, bonesNames, rootBoneName);
				if (found)
				{
					TransformAssimpSkeletonToEngineSkeleton(rootNode, rootJoint, rootBoneName, jointsIndexesMap);
				}
			}
			
			mesh = TransformAssimpMeshToEngineMesh(assimpScene->mMeshes, assimpScene->mNumMeshes, jointsIndexesMap);

			if (assimpScene->HasMaterials())
			{
				TransformAssimpMaterialsToEngineMaterials(assimpScene, mesh);
			}
		}

		if (assimpScene->HasAnimations())
		{
			TransformAssimpAnimationsToEngineAnimations(assimpScene->mAnimations, assimpScene->mNumAnimations, animation, rootBoneName);
		}
	}

	aiReleaseImport(assimpScene);

	return mesh;
}

void AssimpLoader::TransformAssimpMaterialsToEngineMaterials(const aiScene* assimpScene, Mesh* mesh)
{
	std::string textureName;

	for (unsigned int m = 0; m < assimpScene->mNumMaterials; ++m)
	{
		aiMaterial* material = assimpScene->mMaterials[m];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			bool hasFilename = TransformAssimpTextureToEngineTexture(material, aiTextureType_DIFFUSE, textureName);
			if (hasFilename)
			{
				mesh->SetDiffuseTextureName(textureName);
			}
		}
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			bool hasFilename = TransformAssimpTextureToEngineTexture(material, aiTextureType_NORMALS, textureName);
			if (hasFilename)
			{
				mesh->SetNormalMapTextureName(textureName);
			}
		}
	}
}

bool AssimpLoader::TransformAssimpTextureToEngineTexture(const aiMaterial* material, aiTextureType type, std::string& filename)
{
	aiString path("");
	bool hasPath = material->GetTexture(type, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS;

	if (hasPath)
	{
		filename = std::string(path.C_Str());
	}

	return hasPath;
}

void AssimpLoader::ReadAllBonesNames(aiMesh** meshes, unsigned int numMeshes, std::map<std::string, int>& bonesNames)
{
	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		aiMesh* mesh = meshes[i];
		for (unsigned int b = 0; b < mesh->mNumBones; ++b)
		{
			aiBone* bone = mesh->mBones[b];
			bonesNames[std::string(bone->mName.C_Str())]++;
		}
	}
}

bool AssimpLoader::FindFirstSkeletonBoneName(const aiNode* rootNode, std::map<std::string, int>& bonesNames, std::string& boneName)
{
	std::string name = rootNode->mName.C_Str();
	std::map<std::string, int>::iterator it = bonesNames.find(name);
	bool found = it != bonesNames.end();
	if (found)
	{
		Log(Log::LOG_INFO) << "Root bone name found: " << name << "\n";
		boneName = it->first;
		return true;
	}
	else
	{
		for (unsigned int i = 0; i < rootNode->mNumChildren; ++i)
		{
			const aiNode* node = rootNode->mChildren[i];
			found = FindFirstSkeletonBoneName(node, bonesNames, boneName);
			if (found)
			{
				return true;
			}
		}
	}
	
	return false;
}

void AssimpLoader::TransformAssimpAnimationsToEngineAnimations(aiAnimation** animations, unsigned int numAnimations, Animation** animation, const std::string& rootJointName)
{
	for (unsigned int i = 0; i < numAnimations; ++i)
	{
		aiAnimation* assimpAnimation = animations[i];
		
		std::multimap<float, KeyFrame*> keyFramesMap;
		std::vector<KeyFrame*> keyFrameList;

		//for a single joint we have numChannels == numFrames
		for (unsigned int j = 0; j < assimpAnimation->mNumChannels; ++j)
		{
			aiNodeAnim* channel = assimpAnimation->mChannels[j];
			
			for (unsigned int k = 0; k < channel->mNumPositionKeys; ++k)
			{
				aiVectorKey position = channel->mPositionKeys[k];
				aiQuatKey rotation = channel->mRotationKeys[k];
				//aiVectorKey scale = channel->mScalingKeys[k];
				float timestamp = static_cast<float>(position.mTime);
				assert(position.mTime == rotation.mTime);
				KeyFrame* keyFrame = nullptr;
				std::multimap<float, KeyFrame*>::iterator it = keyFramesMap.find(timestamp);
				if (it == keyFramesMap.end())
				{
					keyFrame = new KeyFrame();
					keyFrame->SetTimestamp(timestamp);
					keyFramesMap.insert(std::make_pair(timestamp, keyFrame));
					keyFrameList.push_back(keyFrame);
				}
				else
				{
					keyFrame = it->second;
				}

				JointTransform* joint = new JointTransform( 
					glm::vec3(position.mValue.x, position.mValue.y, position.mValue.z), 
					glm::quat(rotation.mValue.w, rotation.mValue.x, rotation.mValue.y, rotation.mValue.z)
				);
				glm::mat4x4 matrix = joint->GetLocalTransform();
				std::string name(channel->mNodeName.C_Str());

				if (name == rootJointName)
				{
					matrix = CORRECTION_MATRIX * matrix;
				}

				joint->SetLocalTransform(matrix);
				keyFrame->AddJointTransform(name, joint);
			}
		}

		std::string name(assimpAnimation->mName.C_Str());
		float duration = static_cast<float>(assimpAnimation->mDuration);

		if (name.empty())
		{
			name = "animation_" + std::to_string(i);
		}

		std::sort(keyFrameList.begin(), keyFrameList.end(), [](const KeyFrame* a, const KeyFrame* b) -> bool
		{
			return a->GetTimestamp() < b->GetTimestamp();
		});
		(*animation) = new Animation(name, duration, keyFrameList);
	}
}

Mesh* AssimpLoader::TransformAssimpMeshToEngineMesh(aiMesh** meshes, unsigned int numMeshes, std::map<std::string, int>& joints)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;

	for (unsigned int i = 0; i < numMeshes; ++i)
	{
		const aiMesh* assimpMesh = meshes[i];
		if (assimpMesh->HasPositions())
		{
			TransformAssimpPositionToEngineVertex(assimpMesh->mVertices, assimpMesh->mNumVertices, vertices);
		}
		if (assimpMesh->HasNormals())
		{
			TransformAssimpPositionToEngineVertex(assimpMesh->mNormals, assimpMesh->mNumVertices, normals);
		}
		if (assimpMesh->HasTextureCoords(0))
		{
			TransformAssimpUVsToEngineUV(assimpMesh->mTextureCoords, assimpMesh->mNumVertices, uvs);
		}
		if (assimpMesh->HasFaces())
		{
			TransformAssimpFacesToEngineIndices(assimpMesh->mFaces, assimpMesh->mNumFaces, indices);
		}

		Mesh* mesh = new Mesh(vertices, uvs, indices, normals);

		if (assimpMesh->HasBones())
		{
			TransformAssimpBonesToEngineWeights(assimpMesh->mBones, assimpMesh->mNumBones, mesh, joints);
		}

		return mesh;
	}

	return nullptr;
}

void AssimpLoader::TransformAssimpBonesToEngineWeights(aiBone** bones, unsigned int numBones, Mesh* mesh, std::map<std::string, int>& joints)
{
	for (unsigned int i = 0; i < numBones; ++i)
	{
		aiBone* bone = bones[i];
		int jointId = joints[std::string(bone->mName.C_Str())];
		for (unsigned int j = 0; j < bone->mNumWeights; ++j)
		{
			aiVertexWeight weight = bone->mWeights[j];
			mesh->AddVertexWeightToVertex(weight.mVertexId, weight.mWeight);
			mesh->AddJointIdToVertex(weight.mVertexId, jointId);
		}
	}
}

void AssimpLoader::TransformAssimpFacesToEngineIndices(aiFace* faces, unsigned int numFaces, std::vector<unsigned int>& indices)
{
	for (unsigned int i = 0; i < numFaces; ++i)
	{
		aiFace face = faces[i];

		assert(face.mNumIndices == 3);
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
}

void AssimpLoader::TransformAssimpUVsToEngineUV(aiVector3D *const textcoords[8], unsigned int numUvs, std::vector<glm::vec2>& uvs)
{
	for (unsigned int i = 0; i < numUvs; ++i)
	{
		const aiVector3D uv = textcoords[0][i];
		uvs.push_back(glm::vec2(uv.x, 1.0f - uv.y));
	}
}

void AssimpLoader::TransformAssimpPositionToEngineVertex(const aiVector3D* positions, unsigned int numPositions, std::vector<glm::vec3>& vertices)
{
	for (unsigned int i = 0; i < numPositions; ++i)
	{
		const aiVector3D position = positions[i];
		glm::vec3 newVertex = CORRECTION_MATRIX * glm::vec4(position.x, position.y, position.z, 1.0) ;
		vertices.push_back(newVertex);
	}
}

void AssimpLoader::TransformAssimpSkeletonToEngineSkeleton(const aiNode* rootNode, Joint** rootJoint, const std::string& rootBoneName, std::map<std::string, int>& joints)
{
	int index = 0;

	rootNode = FindFirstSkeletonNode(rootNode, rootBoneName);
	if (rootNode != nullptr)
	{
		*rootJoint = TransformAssimpSkeletonNodeToJoint(rootNode, &index, joints, rootBoneName);
	}
	else
	{
		Log(Log::LOG_ERROR) << "First note skeleton not found %s" << rootBoneName << "\n";
	}
}

const aiNode* AssimpLoader::FindFirstSkeletonNode(const aiNode* rootNode, const std::string& rootBoneName)
{
	const aiNode* foundNode = nullptr;

	std::string nodeName = std::string(rootNode->mName.C_Str());
	if (nodeName == rootBoneName)
	{
		foundNode = rootNode;
	}
	else
	{
		for (unsigned int i = 0; i < rootNode->mNumChildren; ++i)
		{
			foundNode = FindFirstSkeletonNode(rootNode->mChildren[i], rootBoneName);
			if (foundNode)
			{
				break;
			}
		}
	}
	return foundNode;
}

Joint* AssimpLoader::TransformAssimpSkeletonNodeToJoint(const aiNode* rootNode, int *index, std::map<std::string, int>& joints, const std::string& rootJointName)
{
	std::string nodeName = std::string(rootNode->mName.C_Str());
	
	glm::mat4x4 matrix = AssimpMatrix4x4ToGlmMatrix(rootNode->mTransformation);
	if (nodeName == rootJointName)
	{
		matrix = CORRECTION_MATRIX * matrix;
	}
	Joint* joint = new Joint(*index, nodeName, matrix);
	joints.insert(std::make_pair(joint->GetName(), *index));

	for (unsigned int i = 0; i < rootNode->mNumChildren; ++i)
	{
		const aiNode* childNode = rootNode->mChildren[i];
		*index = *index + 1;
		Joint* child = TransformAssimpSkeletonNodeToJoint(childNode, index, joints, rootJointName);
		joint->AddChild(child);
	}
	return joint;
}

glm::mat4 AssimpLoader::AssimpMatrix4x4ToGlmMatrix(const aiMatrix4x4 &from)
{
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}