#pragma once

#include <fbxsdk.h>
#include <string>

class Mesh;

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	static Mesh* LoadModel(const std::string& filename);

private:
	static Mesh* LoadMesh(FbxScene* scene);
	static void LoadMaterials(FbxScene* scene, Mesh* mesh);
};

