#pragma once
#include "../rapidxml/rapidxml.hpp"
#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <map>

class Mesh;
class Joint;
class Animation;

class ColladaLoader
{
	
public:
	ColladaLoader();
	~ColladaLoader();

	static Mesh* LoadModel(const std::string& filename, Animation** animation, Joint** rootJoint);

private:
	static Mesh* LoadMesh(rapidxml::xml_node<> *geometryLibrary);
	static void LoadVertexWeightJoints(rapidxml::xml_node<>* controllersLibrary, Mesh* mesh);
	static void LoadJointsInformation(rapidxml::xml_node<>* controllersLibrary, Joint** rootJoint);
	static void LoadJoint(rapidxml::xml_node<>* node, Joint** rootJoint, std::map<std::string, int>& jointNames);

	static std::string GetFirstValueString(std::string& listValues);
	static glm::mat4 GetMatrix(rapidxml::xml_node<>* node);
	
	static void FillValuesString(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<std::string>& values);
	static void FillValues(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<float>& values);
	static void FillValuesOf2(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<glm::vec2>& values);
	static void FillValuesOf3(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<glm::vec3>& values);
	static void FillWithSourceNames(rapidxml::xml_node<>* meshNode, std::map<std::string, std::string>& sourceNames);
};

