#include "stdafx.h"
#include "ColladaLoader.h"
#include "../resources/models/Mesh.h"
#include "../resources/models/animation/Joint.h"
#include "../resources/models/animation/Animation.h"
#include "../resources/models/animation/KeyFrame.h"
#include "../resources/models/animation/JointTransform.h"

#include "../utils/Log.h"

#include "glm/gtx/transform.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>


glm::mat4x4 ColladaLoader::CORRECTION_MATRIX = glm::mat4();// glm::rotate(glm::mat4x4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

ColladaLoader::ColladaLoader()
{
}

ColladaLoader::~ColladaLoader()
{
}

Mesh* ColladaLoader::LoadModel(const std::string& filename, Animation** animation, Joint** rootJoint)
{
	Log(Log::LOG_INFO) << "Loading Model: " << filename << "\n";

	// Read the xml file into a stringstream
	std::ifstream fileStream(filename);
	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	fileStream.close();

	// Parse the buffer using the xml file parsing library into doc 
	rapidxml::xml_document<> doc;
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	// Find our root node
	rapidxml::xml_node<> * rootNode = doc.first_node("COLLADA");
	
	if (rootNode != nullptr)
	{
		std::multimap<unsigned int, float> weights;
		std::multimap<unsigned int, unsigned int> joints;
		rapidxml::xml_node<>* controllersLibrary = rootNode->first_node("library_controllers");
		LoadVertexWeightJoints(controllersLibrary, weights, joints);

		rapidxml::xml_node<> *geometryLibrary = rootNode->first_node("library_geometries");
		Mesh* mesh = LoadMesh(geometryLibrary, weights, joints);

		LoadJointsInformation(rootNode, rootJoint);

		if (*rootJoint != nullptr)
		{
			rapidxml::xml_node<>* animationsLibrary = rootNode->first_node("library_animations");
			std::string rootJointName = (*rootJoint)->GetName();
			LoadAnimation(animationsLibrary, animation, rootJointName);
		}
		return mesh;
	}

	Log(Log::LOG_ERROR) << "	Error loading Model: " << filename << "\n";

	return nullptr;
}

void ColladaLoader::LoadAnimation(rapidxml::xml_node<>* animationsLibrary, Animation** animation, const std::string& rootJointName)
{
	if (animationsLibrary != nullptr)
	{
		std::multimap<float, KeyFrame*> keyFramesMap;
		std::vector<KeyFrame*> keyFrameList;
		float duration = 0.0f;

		for (rapidxml::xml_node<>* animationNode = animationsLibrary->first_node("animation"); animationNode != nullptr; animationNode = animationNode->next_sibling())
		{
			std::string jointName;
			rapidxml::xml_node<>* channelNode = animationNode->first_node("channel");
			if (channelNode != nullptr)
			{
				rapidxml::xml_attribute<>* attribute = channelNode->first_attribute("target");
				if (attribute != nullptr)
				{
					jointName = attribute->value();
					int separatorIndex = jointName.find("/");
					//Armature_
					jointName = jointName.substr(0, separatorIndex);
				}
			}

			std::map<std::string, std::string> sourceNames;
			FillWithSourceNames(animationNode, sourceNames);

			if (sourceNames.size() > 0)
			{

				std::vector<float> keyFramesTime;
				FillValues(animationNode, sourceNames["INPUT"], keyFramesTime);

				std::vector<float> keyFramesMatrix;
				FillValues(animationNode, sourceNames["OUTPUT"], keyFramesMatrix);

				if (keyFramesMatrix.size() / 16 == keyFramesTime.size())
				{
					for (unsigned int frame = 0; frame < keyFramesTime.size(); frame++)
					{
						float time = keyFramesTime[frame];
						duration = std::max(duration, time);

						KeyFrame* keyFrame = nullptr;

						std::multimap<float, KeyFrame*>::iterator it = keyFramesMap.find(time);
						if (it == keyFramesMap.end())
						{
							keyFrame = new KeyFrame();
							keyFrame->SetTimestamp(time);
							keyFramesMap.insert(std::make_pair(time, keyFrame));
							keyFrameList.push_back(keyFrame);
						}
						else
						{
							keyFrame = it->second;
						}

						glm::mat4x4 matrix = GetMatrix(keyFramesMatrix, 16 * frame);
						//matrix = glm::transpose(matrix);
						if (jointName == rootJointName)
						{
							matrix = CORRECTION_MATRIX * matrix;
						}
						JointTransform* jointTransform = new JointTransform(matrix);
						keyFrame->AddJointTransform(jointName, jointTransform);
					}
				}
				else
				{
					Log(Log::LOG_ERROR) << "	Error loading Animation number frames differ than number of matrices" << "\n";
				}
			}
		}

		//create animation
		(*animation) = new Animation(std::string("animation_0"), duration, keyFrameList);
	}
}

void ColladaLoader::LoadJointsInformation(rapidxml::xml_node<>* collada, Joint** rootJoint)
{
	std::map<std::string, int> jointNamesMap;

	rapidxml::xml_node<>* controllersLibraryNode = collada->first_node("library_controllers");
	if (controllersLibraryNode != nullptr)
	{
		rapidxml::xml_node<>* controllerNode = controllersLibraryNode->first_node("controller");
		if (controllerNode != nullptr)
		{
			rapidxml::xml_node<>* skinNode = controllerNode->first_node("skin");
			if (skinNode != nullptr)
			{
				std::map<std::string, std::string> sourceNames;

				rapidxml::xml_node<>* jointsNode = skinNode->first_node("joints");
				if (jointsNode != nullptr)
				{
					for (rapidxml::xml_node<>* inputNode = jointsNode->first_node("input"); inputNode != nullptr; inputNode = inputNode->next_sibling())
					{
						rapidxml::xml_attribute<> *attribute = inputNode->first_attribute("semantic");
						if (attribute != nullptr)
						{
							std::string inputType = attribute->value();
							attribute = inputNode->first_attribute("source");
							if (attribute != nullptr)
							{
								std::string nameSource = attribute->value();
								sourceNames[inputType] = nameSource.substr(1, nameSource.size());
							}
						}
					}
				}
				std::vector<std::string> jointNames;
				FillValuesString(skinNode, sourceNames["JOINT"], jointNames);
				for (unsigned int i = 0; i < jointNames.size(); ++i)
				{
					jointNamesMap[jointNames[i]] = i;
				}
			}
		}
	}

	rapidxml::xml_node<>* visualScenesController = collada->first_node("library_visual_scenes");
	if (visualScenesController != nullptr)
	{
		rapidxml::xml_node<>* visualSceneNode = visualScenesController->first_node("visual_scene");
		if (visualSceneNode != nullptr)
		{
			for (rapidxml::xml_node<>* node = visualSceneNode->first_node("node"); node != nullptr; node = node->next_sibling())
			{
				LoadJoint(node, rootJoint, jointNamesMap);
			}
		}
	}
}

void ColladaLoader::LoadJoint(rapidxml::xml_node<>* rootNode, Joint** rootJoint, std::map<std::string, int>& jointNames)
{
	for (rapidxml::xml_node<>* node = rootNode->first_node("node"); node != nullptr; node = node->next_sibling())
	{
		rapidxml::xml_attribute<>* attribute = node->first_attribute("type");
		if (attribute != nullptr)
		{
			std::string type = attribute->value();
			if (type == "JOINT")
			{
				attribute = node->first_attribute("sid");
				if(attribute != nullptr)
				{ 
					std::string name = attribute->value();
					unsigned int index = jointNames[name];
					glm::mat4 matrix = GetMatrix(node);
					//matrix = glm::transpose(matrix);
					if (*rootJoint == nullptr)
					{
						matrix = CORRECTION_MATRIX * matrix;
					}
					Joint* joint = new Joint(index, name, matrix);

					*rootJoint == nullptr ? *rootJoint = joint : (*rootJoint)->AddChild(joint);
					LoadJoint(node, &joint, jointNames);
				}
			}
		}
	}
}

glm::mat4 ColladaLoader::GetMatrix(std::vector<float>& values, unsigned int offset)
{
	glm::mat4 matrix;

	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 4; ++c)
		{
			float value = values[c + r * 4 + offset];
			matrix[c][r] = value;
		}
	}

	return matrix;
}

glm::mat4 ColladaLoader::GetMatrix(std::string& values)
{
	glm::mat4 matrix;

	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 4; ++c)
		{
			float value = static_cast<float>(atof(GetFirstValueString(values).c_str()));
			matrix[c][r] = value;
		}
	}

	return matrix;
}

glm::mat4 ColladaLoader::GetMatrix(rapidxml::xml_node<>* node)
{
	glm::mat4 matrix;

	rapidxml::xml_node<>* matrixNode = node->first_node("matrix");
	if (matrixNode != nullptr)
	{
		std::string values = matrixNode->value();
		matrix = GetMatrix(values);
	}

	return matrix;
}

void ColladaLoader::LoadVertexWeightJoints(rapidxml::xml_node<>* controllersLibrary, std::multimap<unsigned int, float>& weights, std::multimap<unsigned int, unsigned int>& joints)
{
	if (controllersLibrary != nullptr)
	{
		rapidxml::xml_node<>* controllerNode = controllersLibrary->first_node("controller");
		if (controllerNode != nullptr)
		{
			rapidxml::xml_node<>* skinNode = controllerNode->first_node("skin");
			if (skinNode != nullptr)
			{
				std::map<std::string, std::string> sourceNames;
				FillWithSourceNames(skinNode, sourceNames);

				rapidxml::xml_node<>* vertexWeightsNode = skinNode->first_node("vertex_weights");
				if (vertexWeightsNode != nullptr)
				{
					std::vector<float> weightValues;
					FillValues(skinNode, sourceNames["WEIGHT"], weightValues);

					rapidxml::xml_node<>* vCountNode = vertexWeightsNode->first_node("vcount");
					if (vCountNode != nullptr)
					{
						std::string vCountString = vCountNode->value();
						rapidxml::xml_node<>* vNode = vertexWeightsNode->first_node("v");
						if (vNode != nullptr)
						{
							std::string vString = vNode->value();
							int vertexIndex = 0;
							while (!vCountString.empty())
							{
								unsigned int numVertexWeightPerVertex = atoi(GetFirstValueString(vCountString).c_str());

								int numElementsPerVertex = sourceNames.size();
								assert(numElementsPerVertex == 2);
								for (unsigned int i = 0; i < numVertexWeightPerVertex; ++i)
								{
									unsigned int jointIndex = atoi(GetFirstValueString(vString).c_str());
									unsigned int weightIndex = atoi(GetFirstValueString(vString).c_str());
									weights.insert(std::make_pair(vertexIndex, weightValues[weightIndex]));
									joints.insert(std::make_pair(vertexIndex, jointIndex));
								}
								vertexIndex++;
							}
						}
					}
				}
			}
		}
	}
}

std::string ColladaLoader::GetFirstValueString(std::string& listValues)
{
	std::string value;
	int spaceIndex = listValues.find(" ");
	if (spaceIndex == std::string::npos)
	{
		value = listValues.substr(0, listValues.size());
		listValues.clear();
	}
	else
	{
		value = listValues.substr(0, spaceIndex);
		listValues = listValues.substr(spaceIndex + 1, listValues.size() - spaceIndex);
	}
	return value;
}

Mesh* ColladaLoader::LoadMesh(rapidxml::xml_node<> *geometryLibrary, std::multimap<unsigned int, float>& weights, std::multimap<unsigned int, unsigned int>& joints)
{
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<int> verticesIndices;

	for (rapidxml::xml_node<>* geometryNode = geometryLibrary->first_node("geometry"); geometryNode != nullptr; geometryNode = geometryNode->next_sibling())
	{
		for (rapidxml::xml_node<>* meshNode = geometryNode->first_node("mesh"); meshNode != nullptr; meshNode = meshNode->next_sibling())
		{
			std::map<std::string, std::string> sourceNames;
			FillWithSourceNames(meshNode, sourceNames);
			
			std::vector<glm::vec3> tempNormals;
			std::vector<glm::vec2> tempUvs;
			std::vector<glm::vec3> tempVertices;

			FillValuesOf3(meshNode, sourceNames["VERTEX"], tempVertices);
			FillValuesOf3(meshNode, sourceNames["NORMAL"], tempNormals);
			FillValuesOf2(meshNode, sourceNames["TEXCOORD"], tempUvs);
			//TODO not reading COLOR information
			
			rapidxml::xml_node<>* polylistNode = meshNode->first_node("polylist");
			if (polylistNode == nullptr)
			{
				polylistNode = meshNode->first_node("triangles");
			}
			if (polylistNode != nullptr)
			{
				unsigned int numInputs = 0;
				for (rapidxml::xml_node<>* inputNode = polylistNode->first_node("input"); inputNode != nullptr; inputNode = inputNode->next_sibling())
				{
					std::string inputNodeName = inputNode->name();
					if (inputNodeName == "input")
					{
						numInputs++;
					}
				}

				std::string vertexInformationList = polylistNode->first_node("p")->value();

				unsigned int element = 0;
				int indexFirstSpace = 1;
				bool hasTexureCoordinates = tempUvs.size() > 0;
				bool hasNormals = tempNormals.size() > 0;
				while (!vertexInformationList.empty() && indexFirstSpace > 0)
				{
					indexFirstSpace = vertexInformationList.find(" ");
					std::string value = vertexInformationList.substr(0, indexFirstSpace);
					unsigned int index = atoi(value.c_str());
					if (element % numInputs == 0)
					{
						indices.push_back(index);

						glm::vec3 newVertex = CORRECTION_MATRIX * glm::vec4(tempVertices[index], 1.0);
						vertices.push_back(newVertex);
						verticesIndices.push_back(index);
					}
					else if (hasNormals && element % numInputs == 1)
					{
						glm::vec3 newNormal = CORRECTION_MATRIX * glm::vec4(tempNormals[index], 1.0);
						normals.push_back(newNormal);
					}
					else if (hasTexureCoordinates && element % numInputs == 2)
					{
						uvs.push_back(tempUvs[index]);
					}
					vertexInformationList = vertexInformationList.substr(indexFirstSpace + 1, vertexInformationList.size() - indexFirstSpace);
					element++;
				}
			}
		}
	}

	Mesh* mesh = new Mesh(vertices, uvs, indices, normals);
	typedef std::multimap<unsigned int, unsigned int>::iterator JointsIterator;
	for (unsigned int i = 0; i < verticesIndices.size(); ++i)
	{
		std::pair<JointsIterator, JointsIterator> result = joints.equal_range(verticesIndices[i]);
		for (JointsIterator it = result.first; it != result.second; it++)
		{
			mesh->AddJointIdToVertex(i, it->second);
		}
	}
	typedef std::multimap<unsigned int, float>::iterator WeightsIterator;
	for (unsigned int i = 0; i < verticesIndices.size(); ++i)
	{
		std::pair<WeightsIterator, WeightsIterator> result = weights.equal_range(verticesIndices[i]);
		for (WeightsIterator it = result.first; it != result.second; it++)
		{
			mesh->AddVertexWeightToVertex(i, it->second);
		}
	}

	return mesh;
}

void ColladaLoader::FillValuesString(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<std::string>& values)
{
	values.clear();

	for (rapidxml::xml_node<>* sourceNode = meshNode->first_node("source"); sourceNode != nullptr; sourceNode = sourceNode->next_sibling())
	{
		if (sourceNode != nullptr)
		{
			rapidxml::xml_attribute<> *attribute = sourceNode->first_attribute("id");
			if (attribute != nullptr)
			{
				std::string attributeValue = attribute->value();

				if (attributeValue == name)
				{
					rapidxml::xml_node<>* nameArrayNode = sourceNode->first_node("Name_array");
					if (nameArrayNode != nullptr)
					{
						std::string valuesString = nameArrayNode->value();
						unsigned int numElements = atoi(nameArrayNode->first_attribute("count")->value());
						for (unsigned int i = 0; i < numElements; i++)
						{
							std::string value = GetFirstValueString(valuesString);
							values.push_back(value);
						}
						break;
					}
				}
			}
		}
	}
}

void ColladaLoader::FillValues(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<float>& values)
{
	values.clear();

	for (rapidxml::xml_node<>* sourceNode = meshNode->first_node("source"); sourceNode != nullptr; sourceNode = sourceNode->next_sibling())
	{
		if (sourceNode != nullptr)
		{
			rapidxml::xml_attribute<> *attribute = sourceNode->first_attribute("id");
			if (attribute != nullptr)
			{
				std::string attributeValue = attribute->value();

				if (attributeValue == name)
				{
					rapidxml::xml_node<>* floatArrayNode = sourceNode->first_node("float_array");
					if (floatArrayNode != nullptr)
					{
						std::string valuesString = floatArrayNode->value();
						unsigned int numElements = atoi(floatArrayNode->first_attribute("count")->value());
						for (unsigned int i = 0; i < numElements; i++)
						{
							float floatValue = static_cast<float>(atof(GetFirstValueString(valuesString).c_str()));
							values.push_back(floatValue);
						}
						break;
					}
				}
			}
		}
	}
}

void ColladaLoader::FillValuesOf2(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<glm::vec2>& values)
{
	values.clear();

	for (rapidxml::xml_node<>* sourceNode = meshNode->first_node("source"); sourceNode != nullptr; sourceNode = sourceNode->next_sibling())
	{
		if (sourceNode != nullptr)
		{
			rapidxml::xml_attribute<> *attribute = sourceNode->first_attribute("id");
			if (attribute != nullptr)
			{
				std::string attributeValue = attribute->value();

				if (attributeValue == name)
				{
					rapidxml::xml_node<>* floatArrayNode = sourceNode->first_node("float_array");
					if (floatArrayNode != nullptr)
					{
						std::string valuesString = floatArrayNode->value();
						unsigned int numElements = atoi(floatArrayNode->first_attribute("count")->value());
						for (unsigned int i = 0; i < numElements; i = i + 2)
						{
							float floatValues[2];
							for (unsigned int j = 0; j < 2; ++j)
							{
								floatValues[j] = static_cast<float>(atof(GetFirstValueString(valuesString).c_str()));
							}
							values.push_back(glm::vec2(floatValues[0], floatValues[1]));
						}
						break;
					}
				}
			}
		}
	}
}

void ColladaLoader::FillValuesOf3(rapidxml::xml_node<>* meshNode, std::string& name, std::vector<glm::vec3>& values)
{
	values.clear();

	for (rapidxml::xml_node<>* sourceNode = meshNode->first_node("source"); sourceNode != nullptr; sourceNode = sourceNode->next_sibling())
	{
		if (sourceNode != nullptr)
		{
			rapidxml::xml_attribute<> *attribute = sourceNode->first_attribute("id");
			if (attribute != nullptr)
			{
				std::string attributeValue = attribute->value();

				if (attributeValue == name)
				{
					rapidxml::xml_node<>* floatArrayNode = sourceNode->first_node("float_array");
					if (floatArrayNode != nullptr)
					{
						std::string valuesString = floatArrayNode->value();
						unsigned int numElements = atoi(floatArrayNode->first_attribute("count")->value());
						for (unsigned int i = 0; i < numElements; i = i + 3)
						{
							float floatValues[3];
							for (unsigned int j = 0; j < 3; ++j)
							{
								floatValues[j] = static_cast<float>(atof(GetFirstValueString(valuesString).c_str()));
							}
							values.push_back(glm::vec3(floatValues[0], floatValues[1], floatValues[2]));
						}
						break;
					}
				}
			}
		}
	}
}

void ColladaLoader::FillWithSourceNames(rapidxml::xml_node<>* node, std::map<std::string, std::string>& sourceNames)
{
	std::vector<std::string> names = {"polylist", "triangles", "vertex_weights", "sampler"};
	rapidxml::xml_node<>* polyList = nullptr;
	unsigned int i = 0;
	while(polyList == nullptr && i < names.size())
	{
		polyList = node->first_node(names[i].c_str());
		++i;
	}
	
	if (polyList != nullptr)
	{
		for (rapidxml::xml_node<>* inputNode = polyList->first_node("input"); inputNode != nullptr; inputNode = inputNode->next_sibling())
		{
			rapidxml::xml_attribute<> *attribute = inputNode->first_attribute("semantic");
			if (attribute != nullptr)
			{
				std::string inputType = attribute->value();
				attribute = inputNode->first_attribute("source");
				if (attribute != nullptr)
				{
					std::string nameSource = attribute->value();
					sourceNames[inputType] = nameSource.substr(1, nameSource.size());
				}
			}
		}
		rapidxml::xml_node<>* verticesNode = node->first_node("vertices");
		if (verticesNode != nullptr)
		{
			rapidxml::xml_attribute<> *attribute = verticesNode->first_attribute("id");
			std::string vertexValue = attribute->value();

			if (attribute != nullptr && sourceNames["VERTEX"] == vertexValue)
			{
				attribute = verticesNode->first_node("input")->first_attribute("source");
				if (attribute != nullptr)
				{
					std::string newName = attribute->value();
					sourceNames["VERTEX"] = newName.substr(1, newName.size());
				}
			}
		}
	}
}