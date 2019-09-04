#pragma once
#include <glm/glm.hpp>
#include "../utils/serializer/ISerializable.h"

class Transformation : public core::utils::ISerializable
{
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
	glm::mat4 mModelMatrix;
	bool mIsDirty;

public:
	Transformation();
	Transformation(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	~Transformation();

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);

	const glm::mat4& GetModelMatrix();

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;
};

