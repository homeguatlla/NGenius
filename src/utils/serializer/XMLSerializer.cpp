#include "stdafx.h"
#include "XMLSerializer.h"
#include "../../rapidxml/rapidxml_print.hpp"
#include <sstream>
#include <fstream>

namespace core
{
	namespace utils
	{
		XMLSerializer::XMLSerializer()
		{
			//adding declaration of xml file
			rapidxml::xml_node<>* declaration = mXMLDocument.allocate_node(rapidxml::node_declaration);
			declaration->append_attribute(mXMLDocument.allocate_attribute("version", "1.0"));
			declaration->append_attribute(mXMLDocument.allocate_attribute("encoding", "utf-8"));
			mXMLDocument.append_node(declaration);
		}

		XMLSerializer::~XMLSerializer()
		{
		}

		void XMLSerializer::Save(const std::string& filename)
		{
			std::ofstream fileStream(filename);
			std::string xmlString;
			rapidxml::print(std::back_inserter(xmlString), mXMLDocument);
			fileStream << xmlString;
			fileStream.close();
			mXMLDocument.clear();
		}

		void XMLSerializer::BeginAttribute(const std::string& name)
		{
			rapidxml::xml_node<>* node = mXMLDocument.allocate_node(rapidxml::node_element, AllocateString(name));

			if (mCurrentXMLNode == nullptr)
			{
				mXMLDocument.append_node(node);
			}
			else
			{
				mCurrentXMLNode->append_node(node);
			}
			mCurrentXMLNode = node;
		}

		void XMLSerializer::WriteNumberOfElements(unsigned int numElements)
		{
			/*Not used for XMLSerializer*/
		}

		void XMLSerializer::WriteParameter(const std::string& name, const std::string& value)
		{
			assert(mCurrentXMLNode != nullptr);
			mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(name), AllocateString(value)));
		}

		void XMLSerializer::WriteParameter(const std::string& name, int value, const std::string& readable)
		{
			assert(mCurrentXMLNode != nullptr);

			mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(name), AllocateString(std::to_string(value))));
		}

		void XMLSerializer::WriteParameter(const std::string& name, const bool value)
		{
			assert(mCurrentXMLNode != nullptr);

			mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(name), AllocateString(std::to_string(value))));
		}

		void XMLSerializer::WriteParameter(const std::string& name, const unsigned int value)
		{
			assert(mCurrentXMLNode != nullptr);

			mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(name), AllocateString(std::to_string(value))));
		}

		void XMLSerializer::WriteParameter(const std::string& name, const float value)
		{
			assert(mCurrentXMLNode != nullptr);

			mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(name), AllocateString(std::to_string(value))));
		}

		void XMLSerializer::WriteParameter(const std::string& name, const glm::vec3& value)
		{
			assert(mCurrentXMLNode != nullptr);
			BeginAttribute(name);
				mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(std::string("X")), AllocateString(std::to_string(value.x))));
				mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(std::string("Y")), AllocateString(std::to_string(value.y))));
				mCurrentXMLNode->append_attribute(mXMLDocument.allocate_attribute(AllocateString(std::string("Z")), AllocateString(std::to_string(value.z))));
			EndAttribute();
		}

		void XMLSerializer::EndAttribute()
		{
			assert(mCurrentXMLNode != nullptr);
			if (mCurrentXMLNode != nullptr && mCurrentXMLNode->parent() != nullptr)
			{
				mCurrentXMLNode = mCurrentXMLNode->parent();
			}
		}

		char* XMLSerializer::AllocateString(const std::string& string)
		{
			return mXMLDocument.allocate_string(string.c_str());
		}
	}
}