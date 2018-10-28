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


glm::mat4x4 AssimpLoader::CORRECTION_MATRIX = glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

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

	if (assimpScene != nullptr && !(assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		aiNode* rootNode = assimpScene->mRootNode;
		std::map<std::string, int> jointsIndexesMap;

		if (rootNode != nullptr)
		{
			TransformAssimpSkeletonToEngineSkeleton(rootNode, rootJoint, jointsIndexesMap);
		}

		Mesh* mesh = nullptr;

		if (assimpScene->HasMeshes())
		{
			mesh = TransformAssimpMeshToEngineMesh(assimpScene->mMeshes, assimpScene->mNumMeshes, jointsIndexesMap);
		}

		if (assimpScene->HasAnimations())
		{
			TransformAssimpAnimationsToEngineAnimations(assimpScene->mAnimations, assimpScene->mNumAnimations, animation);
		}

		return mesh;
	}

	aiReleaseImport(assimpScene);

	return nullptr;
}

void AssimpLoader::TransformAssimpAnimationsToEngineAnimations(aiAnimation** animations, unsigned int numAnimations, Animation** animation)
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

				std::string name(channel->mNodeName.C_Str());
				keyFrame->AddJointTransform(name, joint);
			}
		}

		std::string name(assimpAnimation->mName.C_Str());
		float duration = static_cast<float>(assimpAnimation->mDuration);

		if (name.empty())
		{
			name = "animation_" + std::to_string(i);
		}

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
		for (unsigned int j = 0; j < bone->mNumWeights && j < 4; ++j)
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
		uvs.push_back(glm::vec2(uv.x, uv.y));
	}
}

void AssimpLoader::TransformAssimpPositionToEngineVertex(const aiVector3D* positions, unsigned int numPositions, std::vector<glm::vec3>& vertices)
{
	for (unsigned int i = 0; i < numPositions; ++i)
	{
		const aiVector3D position = positions[i];
		glm::vec3 newVertex = glm::vec4(position.x, position.y, position.z, 1.0) * CORRECTION_MATRIX;
		vertices.push_back(newVertex);
	}
}

void AssimpLoader::TransformAssimpSkeletonToEngineSkeleton(const aiNode* rootNode, Joint** rootJoint, std::map<std::string, int>& joints)
{
	int index = 0;

	*rootJoint = TransformAssimpSkeletonNodeToJoint(rootNode, &index, joints);
}

Joint* AssimpLoader::TransformAssimpSkeletonNodeToJoint(const aiNode* rootNode, int *index, std::map<std::string, int>& joints)
{
	Joint* joint = new Joint(*index, std::string(rootNode->mName.C_Str()), AssimpMatrix4x4ToGlmMatrix(rootNode->mTransformation));
	joints.insert(std::make_pair(joint->GetName(), *index));
	for (unsigned int i = 0; i < rootNode->mNumChildren; ++i)
	{
		const aiNode* childNode = rootNode->mChildren[i];
		*index = *index + 1;
		Joint* child = TransformAssimpSkeletonNodeToJoint(childNode, index, joints);
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