#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace NGenius
{
	class NMath
	{
		static glm::quat FromEulerAnglesToQuaternion(const glm::vec3& euler) {
			// Assuming the angles are in radians.
			double c1 = glm::cos(euler.y);
			double s1 = glm::sin(euler.y);
			double c2 = glm::cos(euler.x);
			double s2 = glm::sin(euler.x);
			double c3 = glm::cos(euler.z);
			double s3 = glm::sin(euler.z);
			float w = glm::sqrt(1.0 + c1 * c2 + c1 * c3 - s1 * s2 * s3 + c2 * c3) / 2.0;
			double w4 = (4.0 * w);
			float x = (c2 * s3 + c1 * s3 + s1 * s2 * c3) / w4;
			float y = (s1 * c2 + s1 * c3 + c1 * s2 * s3) / w4;
			float z = (-s1 * s3 + c1 * s2 * c3 + s2) / w4;

			return glm::quat(w, x, y, z);
		}

		static glm::vec3 FromQuatToEulerAngles(const glm::quat& q) {
			double sqw = q.w * q.w;
			double sqx = q.x * q.x;
			double sqy = q.y * q.y;
			double sqz = q.z * q.z;
			double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
			double test = q.x * q.y + q.z * q.w;

			float heading, attitude, bank;

			if (test > 0.499 * unit) { // singularity at north pole
				heading = 2 * atan2(q.x, q.w);
				attitude = glm::pi<float>() / 2.0f;
				bank = 0;
				return glm::vec3(attitude, heading, bank);
			}

			if (test < -0.499 * unit) { // singularity at south pole
				heading = -2 * atan2(q.x, q.w);
				attitude = -glm::pi<float>() / 2.0f;
				bank = 0;
				return glm::vec3(attitude, heading, bank);
			}

			heading = atan2(2 * q.y * q.w - 2 * q.x * q.z, sqx - sqy - sqz + sqw);
			attitude = asin(2 * test / unit);
			bank = atan2(2 * q.x * q.w - 2 * q.y * q.z, -sqx + sqy - sqz + sqw);

			return glm::vec3(attitude, heading, bank);
		}

		static bool IsNearlyEqual(const glm::vec3& v1, const glm::vec3& v2, const float epsilon)
		{
			const glm::vec3 epsilon3(epsilon);
			//return glm::epsilonEqual<glm::vec3>(v1, v2, epsilon3);
			return glm::all(glm::lessThan(glm::abs(v1 - v2), epsilon3));
		}

		static bool IsNearlyEqual(float a, float b, const float epsilon = 0.0001f)
		{
			return glm::abs(a - b) < epsilon;
		}
	};
};

