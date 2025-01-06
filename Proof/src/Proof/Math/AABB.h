#pragma once
#include <glm/glm.hpp>
namespace Proof
{
	struct AABB
	{
		glm::vec3 Min, Max;

		AABB()
			: Min(std::numeric_limits<float>::infinity()), Max(-std::numeric_limits<float>::infinity()) {}

		AABB(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max) {}

		glm::vec3 GetCenter()const
		{
			return (Min + Max) * 0.5f;
		}

		glm::vec3 GetHalfSize()const
		{
			return (Min - Max) * 0.5f;
		}
		glm::vec3 GetExtents()const
		{
			auto center = GetCenter();
			return glm::vec3{ Max.x - center.x, Max.y - center.y, Max.z - center.z };
		}
		/*
		AABB ScaleAABB(glm::vec3 scale)
		{
			// Calculate the center of the AABB
			glm::vec3 center = GetCenter();

			// Calculate the half-size (extents) of the AABB
			glm::vec3 halfSize = GetHalfSize();

			// Scale the extents
			glm::vec3 scaledHalfSize = halfSize * scale;

			// Create a new AABB with scaled dimensions
			AABB scaledAABB;
			scaledAABB.Min = center - scaledHalfSize;
			scaledAABB.Max = center + scaledHalfSize;

			return scaledAABB;
		}
		*/

		AABB ScaleAABB(const glm::mat4& transform) const
		{
			// Define the corners of the original AABB
			glm::vec4 corners[8] = {
				glm::vec4(Min.x, Min.y, Min.z, 1.0f),
				glm::vec4(Min.x, Min.y, Max.z, 1.0f),
				glm::vec4(Min.x, Max.y, Min.z, 1.0f),
				glm::vec4(Min.x, Max.y, Max.z, 1.0f),
				glm::vec4(Max.x, Min.y, Min.z, 1.0f),
				glm::vec4(Max.x, Min.y, Max.z, 1.0f),
				glm::vec4(Max.x, Max.y, Min.z, 1.0f),
				glm::vec4(Max.x, Max.y, Max.z, 1.0f)
			};

			// Transform all corners
			glm::vec3 newMin(FLT_MAX), newMax(-FLT_MAX);
			for (const auto& corner : corners) {
				glm::vec4 transformedCorner = transform * corner;
				glm::vec3 transformedCorner3D = glm::vec3(transformedCorner); // Convert to vec3
				newMin = glm::min(newMin, transformedCorner3D);
				newMax = glm::max(newMax, transformedCorner3D);
			}

			// Create a new AABB with the updated bounds
			return { newMin, newMax };
		}

	};
}