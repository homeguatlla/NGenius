#pragma once

#include <glm/glm.hpp>
#include <string>

namespace core
{
	namespace utils
	{
		class IDeserializer
		{
		public:
			IDeserializer() {}
			virtual ~IDeserializer() {}

			virtual void BeginAttribute(const std::string& name) = 0;

			virtual unsigned int ReadNumberOfElements() = 0;

			virtual void ReadParameter(const std::string& name, std::string& value) = 0;
			virtual void ReadParameter(const std::string& name, int& value, std::string& readable) = 0;
			virtual void ReadParameter(const std::string& name, bool& value) = 0;
			virtual void ReadParameter(const std::string& name, unsigned int& value) = 0;
			virtual void ReadParameter(const std::string& name, float& value) = 0;
			virtual void ReadParameter(const std::string& name, glm::vec3& value) = 0;

			virtual void EndAttribute() = 0;
		};
	}
}

