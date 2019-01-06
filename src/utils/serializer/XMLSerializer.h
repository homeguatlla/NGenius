#pragma once
#include "ISerializer.h"
#include "../../rapidxml/rapidxml.hpp"

namespace core
{
	namespace utils
	{
		class XMLSerializer : public ISerializer
		{
		public:
			XMLSerializer();
			virtual ~XMLSerializer();

			void Save(const std::string& filename);

			// Heredado vía ISerializer
			void BeginAttribute(const std::string& name) override;
			void WriteNumberOfElements(unsigned int numElements) override;
			void WriteParameter(const std::string& name, const std::string& value) override;
			void WriteParameter(const std::string& name, int value, const std::string& readable) override;
			void WriteParameter(const std::string& name, const bool value) override;
			void WriteParameter(const std::string& name, const unsigned int value) override;
			void WriteParameter(const std::string& name, const float value) override;
			void WriteParameter(const std::string& name, const glm::vec3& value) override;
			void EndAttribute() override;
		private:
			char* AllocateString(const std::string& string);
		protected:
			rapidxml::xml_document<> mXMLDocument;
			rapidxml::xml_node<>* mCurrentXMLNode;
		};
	}
}