#pragma once
#include "Vector.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include "Math.h"
namespace Proof {
	template< class T=float>
	struct Transform {
		Vector <T> Location;
		Vector <T> Rotation;
		Vector <T> Scale;
		glm::mat4 GetTransfrom() {
			return glm::translate(glm::mat4(1.0f), { Location }) *
				glm::rotate(glm::mat4(1.0f), Math::Radian<T>(Rotation.X), { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), Math::Radian<T>(Rotation.Y), { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), Math::Radian<T>(Rotation.Z), { 0,0,1 })
				* glm::scale(glm::mat4(1.0f), { Scale });
		}
	};
}