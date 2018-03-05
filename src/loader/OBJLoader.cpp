#include "stdafx.h"
#include "OBJLoader.h"
#include "../resources/models/Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Mesh* OBJLoader::LoadModel(const std::string& filename)
{
	std::vector<glm::vec3> tempVertexs;
	std::vector<glm::vec3> tempNormals;
	std::vector<glm::vec2> tempUvs;
	std::vector<Face> faces;

	std::string materialFilename;
	std::string filepath = GetFilepath(filename);

	std::ifstream input(filename);

	if (input.is_open())
	{
		std::string line;
		while (std::getline(input, line))
		{
			if (Read(line, "vt "))
			{
				glm::vec2 vt = ReadVertex2(line);
				tempUvs.push_back(vt);
			} 
			else if (Read(line, "vn "))
			{
				glm::vec3 n = ReadVertex3(line);
				tempNormals.push_back(n);
			}
			else if (Read(line, "v "))
			{
				glm::vec3 v = ReadVertex3(line);
				tempVertexs.push_back(v);
			}
			else if (Read(line, "f "))
			{
				int counter = CountNumber(line, "/");
				Face f;
				if (counter <= 6)
				{
					f = ReadFace3(line);
					faces.push_back(f);
				}
				else
				{
					f = ReadFace4(line);
					Face f2;
					SplitFace(f, f2);

					faces.push_back(f);
					faces.push_back(f2);
				}		
			}
			else if (Read(line, "mtllib "))
			{
				materialFilename = line;
			}
		}

		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> indexes;
		std::vector<glm::vec3> vertexs;

		bool hasNormals = false;

		//CreateRepeatedVertexs(tempVertexs, faces);

		//TODO improve it with 3 loops instead of 1 with ifs
		for (Face f : faces)
		{
			assert(f.vertex.x > 0 && f.vertex.y > 0); //vertex and texture information
			if (f.vertex.x > tempVertexs.size() || f.vertex.y > tempVertexs.size())
			{
				continue;
			}
			indexes.push_back(f.vertex.x);
			indexes.push_back(f.vertex.y);
			if (f.vertex.z > 0)
			{
				//there is normal information
				indexes.push_back(f.vertex.z);
				hasNormals = true;
			}

			vertexs.push_back(tempVertexs[f.vertex.x - 1]);
			vertexs.push_back(tempVertexs[f.vertex.y - 1]);
			if (hasNormals)
			{
				vertexs.push_back(tempVertexs[f.vertex.z - 1]);
			}

			if (!tempUvs.empty())
			{
				uvs.push_back(tempUvs[f.texture.x - 1]);
				uvs.push_back(tempUvs[f.texture.y - 1]);
				if (hasNormals)
				{
					uvs.push_back(tempUvs[f.texture.z - 1]);
				}
			}

			if (!tempNormals.empty())
			{
				normals.push_back(tempNormals[f.normal.x - 1]);
				normals.push_back(tempNormals[f.normal.y - 1]);
				normals.push_back(tempNormals[f.normal.z - 1]);
			}
		}

		if (vertexs.size() > 0)
		{
			Mesh* model = new Mesh(vertexs, uvs, indexes, normals);

			if (!materialFilename.empty())
			{
				LoadMaterial(model, filepath, materialFilename);
			}

			return model;
		}
	}

	return nullptr;
}

void OBJLoader::LoadMaterial(Mesh* model, const std::string& path,  const std::string& filename)
{
	std::string diffuseTexture;
	std::string normalMapTexture;
	std::string materialName;

	std::ifstream input(path + filename);

	if (input.is_open())
	{
		std::string line;
		while (std::getline(input, line))
		{
			if (Read(line, "newmtl "))
			{
				materialName = line;
			}
			else if (Read(line, "map_Kd "))
			{
				diffuseTexture = line;
			}
			else if (Read(line, "map_bump "))
			{
				normalMapTexture = line;
			}
		}
	}
	else
	{
		std::cout << "Material filename " << filename << " not found.";
		assert(false);
	}

	model->SetMaterialName(materialName);
	if (!diffuseTexture.empty())
	{
		model->SetDiffuseTextureName(path + diffuseTexture);
	}
	if (!normalMapTexture.empty())
	{
		model->SetNormalMapTextureName(path + normalMapTexture);
	}
}

int OBJLoader::CountNumber(const std::string& line, const std::string search)
{
	int counter = 0;
	int index =  line.find(search, 0);
	while (index != std::string::npos)
	{
		counter++;
		index = line.find(search, index + 1);
	}

	return counter;
}

void OBJLoader::SplitFace(Face& f1, Face& f2)
{
	f2.vertex.x = f1.vertex.z;
	f2.normal.x = f1.normal.z;
	f2.texture.x = f1.texture.z;

	f2.vertex.y = f1.vertex.w;
	f2.normal.y = f1.normal.w;
	f2.texture.y = f1.texture.w;

	f2.vertex.z = f1.vertex.x;
	f2.normal.z = f1.normal.x;
	f2.texture.z = f1.texture.x;
}

void OBJLoader::CreateRepeatedVertexs(std::vector<glm::vec3>& vertexs, std::vector<Face>& faces)
{
	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		Face f = faces[i];

		for (unsigned int j = i + 1; j < faces.size(); ++j)
		{
			Face f2 = faces[j];
			if (f.vertex.x == f2.vertex.x && f.texture.x != f2.texture.x)
			{
				glm::vec3 newVertex = vertexs[f.vertex.x];
				vertexs.push_back(newVertex);
				for (unsigned int k = j + 1;  k < faces.size(); ++k)
				{
					Face f3 = faces[k];
					if (f.vertex.x == f3.vertex.x && f.vertex.x != f3.texture.x)
					{
						f3.vertex.x = vertexs.size() - 1;
					}
				}
			}

			if (f.vertex.y == f2.vertex.y && f.texture.y != f2.texture.y)
			{
				glm::vec3 newVertex = vertexs[f.vertex.y];
				vertexs.push_back(newVertex);
				for (unsigned int k = j + 1; k < faces.size(); ++k)
				{
					Face f3 = faces[k];
					if (f.vertex.y == f3.vertex.y && f.vertex.y != f3.texture.y)
					{
						f3.vertex.y = vertexs.size() - 1;
					}
				}
			}

			if (f.vertex.z == f2.vertex.z && f.texture.z != f2.texture.z)
			{
				glm::vec3 newVertex = vertexs[f.vertex.z];
				vertexs.push_back(newVertex);
				for (unsigned int k = j + 1; k < faces.size(); ++k)
				{
					Face f3 = faces[k];
					if (f.vertex.z == f3.vertex.z && f.vertex.z != f3.texture.z)
					{
						f3.vertex.z = vertexs.size() - 1;
					}
				}
			}
		}
	}
}

OBJLoader::Face OBJLoader::ReadFace3(std::string line)
{
	Face f;

	replace(line.begin(), line.end(), '/', ' ');
	std::stringstream stream(line);
	std::string info;
	
	stream >> f.vertex.x;
	stream >> f.texture.x;
	stream >> f.normal.x;
	stream >> f.vertex.y;
	stream >> f.texture.y;
	stream >> f.normal.y;
	stream >> f.vertex.z;
	stream >> f.texture.z;
	stream >> f.normal.z;

	return f;
}

OBJLoader::Face OBJLoader::ReadFace4(std::string line)
{
	Face f;

	replace(line.begin(), line.end(), '/', ' ');
	std::stringstream stream(line);
	std::string info;

	stream >> f.vertex.x;
	stream >> f.texture.x;
	stream >> f.normal.x;
	stream >> f.vertex.y;
	stream >> f.texture.y;
	stream >> f.normal.y;
	stream >> f.vertex.z;
	stream >> f.texture.z;
	stream >> f.normal.z;
	stream >> f.vertex.w;
	stream >> f.texture.w;
	stream >> f.normal.w;

	return f;
}
