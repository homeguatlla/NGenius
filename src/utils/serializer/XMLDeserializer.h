#pragma once
#include "IDeserializer.h"
#include "../../rapidxml/rapidxml.hpp"

namespace core
{
	namespace utils
	{
		class XMLDeserializer : public IDeserializer
		{
		public:
			XMLDeserializer();
			virtual ~XMLDeserializer();

			void Load(const std::string& filename);

			// Heredado vía ISerializer
			void BeginAttribute(const std::string& name) override;
			unsigned int ReadNumberOfElements() const override;
			void NextAttribute() override;
			std::string GetCurrentNodeName() const override;

			bool ReadParameter(const std::string& name, std::string& value) override;
			bool ReadParameter(const std::string& name, int* value, std::string& readable) override;
			bool ReadParameter(const std::string& name, bool* value) override;
			bool ReadParameter(const std::string& name, unsigned int* value) override;
			bool ReadParameter(const std::string& name, float* value) override;
			bool ReadParameter(const std::string& name, glm::vec3& value) override;
			void EndAttribute() override;
		private:
			bool FillWithValue(const std::string& attributeName, std::string& value);

			std::string mContent;
		protected:
			rapidxml::xml_document<> mXMLDocument;
			rapidxml::xml_node<>* mCurrentXMLNode;
		};
	}
}