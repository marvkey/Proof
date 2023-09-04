#pragma once
#include "Proof/Math/Vector.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>

#define PX_PHYSX_STATIC_LIB
#include <PxPhysicsAPI.h>
#include "CollisonCallback.h"
#include "PhysicsMeshCooker.h"
//solve objexts goign to fast for collisoin
//https://www.youtube.com/watch?v=XvrFQJ3n8Mo
namespace Proof {
	class PhysicsEngine {
	public:
		static physx::PxFoundation* GetFoundation();
		static physx::PxPhysics* GetPhysics();
		static physx::PxPvd* GetPVD();
		static physx::PxDefaultCpuDispatcher* GetCpuDispatcher();
	private:
		static void Init();
		static void Release();
		friend class Application;
	};

	namespace PhysxUtils {
		static Vector PhysxToVector(const physx::PxVec3& physxVector) {
			Vector vec;
			vec.X = physxVector.x;
			vec.Y = physxVector.y;
			vec.Z = physxVector.z;

			return vec;
		}
		static physx::PxVec3 GlmVectorToPhysxVector(const glm::vec3& vector) {
			physx::PxVec3 physxVector;
			physxVector.x = vector.x;
			physxVector.y = vector.y;
			physxVector.z = vector.z;

			return physxVector;
		}
		static glm::vec3 PhysxVectorToGlmVector(const physx::PxVec3& physxVector) {
			glm::vec3 vec;
			vec.x = physxVector.x;
			vec.y = physxVector.y;
			vec.z = physxVector.z;

			return vec;
		}
		static physx::PxVec3 VectorToPhysxVector(const Vector& vector) {
			physx::PxVec3 physxVector;
			physxVector.x = vector.X;
			physxVector.y = vector.Y;
			physxVector.z = vector.Z;

			return physxVector;
		}

		static physx::PxQuat QuatTophysxQuat(const glm::quat& quat) {
			return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
		}
		static glm::quat PhysxQuatToQuat(const physx::PxQuat& quat) {
			return glm::quat(quat.w, quat.x, quat.y, quat.z);
		}
		// values are changed to radians
		// make sure vector values are in degrees

		// make sure quatiernion has values in radians
		// returns vector values in degrees
		static Vector PhysxQuatToVector(const physx::PxQuat& quat) {
			return Math::ConvertQuartToVector(PhysxQuatToQuat(quat));
		}
		static physx::PxMat44 glmMat4ToPhysxMat4(const glm::mat4& mat4) {
			physx::PxMat44 newMat;

			newMat[0][0] = mat4[0][0];
			newMat[0][1] = mat4[0][1];
			newMat[0][2] = mat4[0][2];
			newMat[0][3] = mat4[0][3];

			newMat[1][0] = mat4[1][0];
			newMat[1][1] = mat4[1][1];
			newMat[1][2] = mat4[1][2];
			newMat[1][3] = mat4[1][3];

			newMat[2][0] = mat4[2][0];
			newMat[2][1] = mat4[2][1];
			newMat[2][2] = mat4[2][2];
			newMat[2][3] = mat4[2][3];

			newMat[3][0] = mat4[3][0];
			newMat[3][1] = mat4[3][1];
			newMat[3][2] = mat4[3][2];
			newMat[3][3] = mat4[3][3];


			return newMat;
		}
	}
}