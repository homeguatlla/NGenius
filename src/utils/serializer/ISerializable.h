#pragma once

namespace core
{
	namespace utils
	{
		class ISerializer;
		class IDeserializer;

		class ISerializable
		{
		public:
			virtual ~ISerializable() {}
			virtual void ReadFrom(IDeserializer* source) = 0;
			virtual void WriteTo(ISerializer* destination) = 0;
		};
	}
}

