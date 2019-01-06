#pragma once

#include <glm/glm.hpp>
#include <string>

namespace core
{
	namespace utils
	{
		class ISerializer
		{
		public:
			virtual ~ISerializer() {}
			virtual void BeginAttribute(const std::string& name) = 0;

			virtual void WriteNumberOfElements(unsigned int numElements) = 0;

			virtual void WriteParameter(const std::string& name, const std::string& value) = 0;
			virtual void WriteParameter(const std::string& name, int value, const std::string& readable) = 0;
			virtual void WriteParameter(const std::string& name, const bool value) = 0;
			virtual void WriteParameter(const std::string& name, const unsigned int value) = 0;
			virtual void WriteParameter(const std::string& name, const float value) = 0;
			virtual void WriteParameter(const std::string& name, const glm::vec3& value) = 0;

			virtual void EndAttribute() = 0;
		};
	}
}
