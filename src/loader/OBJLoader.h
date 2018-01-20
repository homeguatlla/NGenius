#pragma once

#include "../../FileReader.h"
#include <vector>


class Model;

class OBJLoader : public FileReader
{
public:
	static Model* LoadModel(const std::string& filename);

private:

	static struct FaceValues
	{
		glm::uvec4 vertex;
		glm::uvec4 texture;
		glm::uvec4 normal;

		FaceValues() {}
	};
	typedef struct FaceValues Face;

	static void LoadMaterial(Model* model, const std::string& path, const std::string& filename);
	static Face ReadFace3(std::string line);
	static Face ReadFace4(std::string line);

	static void CreateRepeatedVertexs(std::vector<glm::vec3>& vertexs, std::vector<Face>& faces);
	static int CountNumber(const std::string& line, const std::string search);
	static void SplitFace(Face& f1, Face& f2);
	
};

