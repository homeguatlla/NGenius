#pragma once

#include "glm/glm.hpp"
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

#include <string>
#include <vector>
#include <map>

class Mesh;
class Animation;
class Joint;
class Mesh;
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
	static void TransformAssimpSkeletonToEngineSkeleton(const aiNode* rootNode, Joint** rootJoint, std::map<std::string, int>& joints);
	static Mesh* TransformAssimpMeshToEngineMesh(aiMesh** meshes, unsigned int numMeshes, std::map<std::string, int>& joints);
	static void TransformAssimpFacesToEngineIndices(aiFace* faces, unsigned int numFaces, std::vector<unsigned int>& indices);
	static void TransformAssimpAnimationsToEngineAnimations(aiAnimation** animations, unsigned int numAnimations, Animation** animation);
	static Joint* TransformAssimpSkeletonNodeToJoint(const aiNode* rootNode, int *index, std::map<std::string, int>& joints);
	static void TransformAssimpBonesToEngineWeights(aiBone** bones, unsigned int numBones, Mesh* mesh, std::map<std::string, int>& joints);

	static glm::mat4 AssimpMatrix4x4ToGlmMatrix(const aiMatrix4x4 &from);
	static void TransformAssimpPositionToEngineVertex(const aiVector3D* positions, unsigned int numPositions, std::vector<glm::vec3>& vertices);
	static void TransformAssimpUVsToEngineUV(aiVector3D *const textcoords[8], unsigned int numUvs, std::vector<glm::vec2>& uvs);
};

