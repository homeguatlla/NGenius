#pragma once
#include "glm/glm.hpp"

#include <fbxsdk.h>
#include <string>
#include <vector>
#include <map>


class Mesh;
class Joint;
class Animation;

class FBXLoader
{
	static std::map<int, int> mControlPoints;
public:
	FBXLoader();
	~FBXLoader();

	static Mesh* LoadModel(const std::string& filename, Animation** animation, Joint** rootJoint);

private:
	static FbxMesh * GetMesh(FbxScene* fbxScene);
	static Mesh* LoadMesh(FbxMesh* fbxMesh);
	static void LoadMaterials(FbxScene* fbxScene, Mesh* mesh);
	static void LoadAnimations(FbxScene* fbxScene, FbxCluster* fbxCluster, Animation** animation);
	static Joint* LoadSkeleton(FbxScene* fbxScene, std::vector<Joint*>& joints);
	static Animation* FillAnimationData(FbxScene* fbxScene, FbxMesh* fbxMesh, std::vector<Joint*>& joints, Mesh* mesh);

	static unsigned int FindJointIndex(const std::string & jointname, std::vector<Joint*>& joints);
	static Joint* ProcessSkeletonHeirarchyre(FbxNode * node, std::vector<Joint*>& joints);

	static void Convert(fbxsdk::FbxAMatrix& matOrigin, glm::mat4& matDestination);
};

