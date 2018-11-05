#pragma once

#include "glm/glm.hpp"
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <assimp/material.h>

#include <string>
#include <vector>
#include <map>

class Mesh;
class Animation;
class Joint;
class Mesh;
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiFace;
struct aiAnimation;
struct aiBone;

class AssimpLoader
{
	static glm::mat4x4 CORRECTION_MATRIX;
public:
	AssimpLoader();
	~AssimpLoader();

	static Mesh* LoadModel(const std::string& filename, Animation** animation, Joint** rootJoint);

private:
	static void TransformAssimpSkeletonToEngineSkeleton(const aiNode* rootNode, Joint** rootJoint, const std::string& rootBoneName, std::map<std::string, int>& joints);
	static Mesh* TransformAssimpMeshToEngineMesh(aiMesh** meshes, unsigned int numMeshes, std::map<std::string, int>& joints);
	static void TransformAssimpFacesToEngineIndices(aiFace* faces, unsigned int numFaces, std::vector<unsigned int>& indices);
	static void TransformAssimpAnimationsToEngineAnimations(aiAnimation** animations, unsigned int numAnimations, Animation** animation, const std::string& rootJointName);
	static Joint* TransformAssimpSkeletonNodeToJoint(const aiNode* rootNode, int *index, std::map<std::string, int>& joints, const std::string& rootJointName);
	static void TransformAssimpBonesToEngineWeights(aiBone** bones, unsigned int numBones, Mesh* mesh, std::map<std::string, int>& joints);
	static void TransformAssimpMaterialsToEngineMaterials(const aiScene* assimpScene, Mesh* mesh);
	static bool TransformAssimpTextureToEngineTexture(const aiMaterial* material, aiTextureType type, std::string& filename);

	static void ReadAllBonesNames(aiMesh** meshes, unsigned int numMeshes, std::map<std::string, int>& bonesNames);
	static const aiNode* FindFirstSkeletonNode(const aiNode* node, const std::string& boneName);
	static bool FindFirstSkeletonBoneName(const aiNode* rootNode, std::map<std::string, int>& bonesNames, std::string& boneName);
	static glm::mat4 AssimpMatrix4x4ToGlmMatrix(const aiMatrix4x4 &from);
	static void TransformAssimpPositionToEngineVertex(const aiVector3D* positions, unsigned int numPositions, std::vector<glm::vec3>& vertices);
	static void TransformAssimpUVsToEngineUV(aiVector3D *const textcoords[8], unsigned int numUvs, std::vector<glm::vec2>& uvs);
};

