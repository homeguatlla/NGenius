#include "stdafx.h"
#include "XMLDeserializer.h"
#include "../../rapidxml/rapidxml_print.hpp"
#include <sstream>
#include <fstream>
#include <algorithm>

namespace core
{
	namespace utils
	{
		XMLDeserializer::XMLDeserializer()
		{
		}

		XMLDeserializer::~XMLDeserializer()
		{
		}

		void XMLDeserializer::Load(const std::string& filename)
		{
			std::ifstream fileStream(filename);
			std::stringstream buffer;
			buffer << fileStream.rdbuf();
			fileStream.close();
			mContent = buffer.str();
			mXMLDocument.parse<0>(&mContent[0]);
		}

		unsigned int XMLDeserializer::ReadNumberOfElements() const
		{
			unsigned int counter = 0;
			if (mCurrentXMLNode != nullptr)
			{
				rapidxml::xml_node<>* node = mCurrentXMLNode->first_node();
				while ( node != nullptr)
				{
					++counter;
					node = node->next_sibling();
				}
			}

			return counter;
		}
		std::string XMLDeserializer::GetCurrentNodeName() const
		{
			if (mCurrentXMLNode != nullptr)
			{
				return std::string(mCurrentXMLNode->name());
			}
			else
			{
				return std::string();
			}
		}

		void XMLDeserializer::BeginAttribute()
		{
			if (mCurrentXMLNode == nullptr)
			{
				mCurrentXMLNode = mXMLDocument.first_node();
			}
			else
			{
				mCurrentXMLNode = mCurrentXMLNode->first_node();
			}
			assert(mCurrentXMLNode != nullptr);
		}

		void XMLDeserializer::BeginAttribute(const std::string& name)
		{
			if (mCurrentXMLNode == nullptr)
			{
				mCurrentXMLNode = mXMLDocument.first_node(name.c_str());
			}
			else
			{
				mCurrentXMLNode = mCurrentXMLNode->first_node(name.c_str());
			}
			assert(mCurrentXMLNode != nullptr);
		}
		
		void XMLDeserializer::NextAttribute()
		{
			assert(mCurrentXMLNode != nullptr);
			if (mCurrentXMLNode->next_sibling() != nullptr)
			{
				mCurrentXMLNode = mCurrentXMLNode->next_sibling();
			}
		}

		void XMLDeserializer::EndAttribute()
		{
			//assert(mCurrentXMLNode != nullptr);
			if (mCurrentXMLNode != nullptr && mCurrentXMLNode->parent() != nullptr)
			{
				mCurrentXMLNode = mCurrentXMLNode->parent();
			}
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, std::string& value)
		{
			assert(mCurrentXMLNode != nullptr);

			bool found = FillWithValue(name, value);

			return found;
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, int* value)
		{
			assert(mCurrentXMLNode != nullptr);

			std::string stringValue;
			bool found = FillWithValue(name, stringValue);

			assert(found);
			if (found)
			{
				*value = std::stoi(stringValue);
			}
			return found;
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, bool* value)
		{
			assert(mCurrentXMLNode != nullptr);
			std::string stringValue;
			bool found = FillWithValue(name, stringValue);
			std::transform(stringValue.begin(), stringValue.end(), stringValue.begin(), ::toupper);

			if (found)
			{
				*value = stringValue == "TRUE" ? true : false;
			}
			return found;
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, unsigned int* value)
		{
			int v = 0;
			bool found = ReadParameter(name, &v);
			*value = v;
			return found;
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, float* value)
		{
			assert(mCurrentXMLNode != nullptr);
			std::string stringValue;

			bool found = FillWithValue(name, stringValue);

			if (found)
			{
				*value = std::stof(stringValue);
			}
			return found;
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, glm::vec4& value)
		{
			assert(mCurrentXMLNode != nullptr);
			std::string stringValue;

			bool found = FillWithValue(name, stringValue);

			if (found)
			{
				std::stringstream stream(stringValue);
				float x, y, z, t;

				stream >> x;
				stream >> y;
				stream >> z;
				stream >> t;

				value = glm::vec4(x, y, z, t);
			}
			return found;
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, glm::vec3& value)
		{
			assert(mCurrentXMLNode != nullptr);
			std::string stringValue;

			bool found = FillWithValue(name, stringValue);

			if (found)
			{
				std::stringstream stream(stringValue);
				float x, y, z;

				stream >> x;
				stream >> y;
				stream >> z;

				value = glm::vec3(x, y, z);
			}
			return found;
		}

		bool XMLDeserializer::ReadParameter(const std::string& name, glm::vec2& value)
		{
			assert(mCurrentXMLNode != nullptr);
			std::string stringValue;

			bool found = FillWithValue(name, stringValue);

			if (found)
			{
				std::stringstream stream(stringValue);
				float x, y;

				stream >> x;
				stream >> y;

				value = glm::vec2(x, y);
			}
			return found;
		}

		bool XMLDeserializer::FillWithValue(const std::string& attributeName, std::string& value)
		{
			if (mCurrentXMLNode != nullptr)
			{
				rapidxml::xml_attribute<> *attribute = mCurrentXMLNode->first_attribute(attributeName.c_str());
				if (attribute != nullptr)
				{
					value = attribute->value();

					return true;
				}
			}
			return false;
		}
	}
}