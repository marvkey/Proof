#include "Proofprch.h"
#include "PhysicsEngine.h"
#include "../Component.h"
#include "../World.h"
#include "../Entity.h"
#include "Proof/Asset/AssetManager.h"
#include "../Material.h"
#include "Proof/Asset/PhysicsMaterialAsset.h"
#include "../CollisionInfo.h"
#include "../Script.h"
#include "../Mesh.h"

namespace Proof {
	namespace Utils {
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
	class PhysicsErrorCallback : public physx::PxDefaultErrorCallback {
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
	};
	static class physx::PxFoundation* s_Foundation;
	static class physx::PxPhysics* s_Physics;
	static class physx::PxDefaultCpuDispatcher* s_Dispatcher;
	static class physx::PxPvd* s_Pvd;
	static class physx::PxDefaultAllocator s_DefaultAllocatorCallback;
	static class PhysicsErrorCallback s_DefaultErrorCallback;
	physx::PxFoundation* PhysicsEngine::GetFoundation()
	{
		return s_Foundation;
	}
	physx::PxPhysics* PhysicsEngine::GetPhysics()
	{
		return s_Physics;
	}
	physx::PxPvd* PhysicsEngine::GetPVD()
	{
		return s_Pvd;
	}
	physx::PxDefaultCpuDispatcher* PhysicsEngine::GetCpuDispatcher()
	{
		return s_Dispatcher;
	}
	void PhysicsEngine::Init() {
		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_DefaultAllocatorCallback,
			s_DefaultErrorCallback);
		if (!s_Foundation)
			PF_CORE_ASSERT(false, "PxCreateFoundation failed!");
		s_Pvd = physx::PxCreatePvd(*s_Foundation);
		physx::PxTolerancesScale scale;
		//scale.length = 100;
		//scale.speed = 981;
		//s_ToleranceScale.length = 100;        // typical length of an object one meter
		//s_ToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
		s_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, scale, true, s_Pvd);
		if (!s_Physics)
			PF_CORE_ASSERT(false, "PxCreateFoundation failed!");
		s_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

		PhysicsMeshCooker::Init();
	}
	void PhysicsEngine::Release()
	{
		PhysicsMeshCooker::Release();
		s_Pvd->release();
		s_Physics->release();
		s_Foundation->release();
		s_Dispatcher->release();
	}
	void PhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		switch (code)
		{
			case physx::PxErrorCode::eDEBUG_INFO:
				PF_EC_INFO("(Physx) Message: {}, File: {}, Line: {}", message, file, line);
				break;

			case physx::PxErrorCode::eDEBUG_WARNING:
			case physx::PxErrorCode::eINVALID_PARAMETER:
			case physx::PxErrorCode::eINVALID_OPERATION:
			case physx::PxErrorCode::eOUT_OF_MEMORY:
			case physx::PxErrorCode::ePERF_WARNING:
				PF_EC_WARN("(Physx) Message: {}, File: {}, Line: {}", message, file, line);
				break;

			case physx::PxErrorCode::eINTERNAL_ERROR:
				PF_EC_ERROR("(Physx) Message: {}, File: {}, Line: {}", message, file, line);
				break;
			case physx::PxErrorCode::eABORT:
				PF_EC_CRITICAL("(Physx) Message: {}, File: {}, Line: {}", message, file, line);
				break;
			default:
				PF_EC_CRITICAL("(Physx Unkown Error) Message: {}, File: {}, Line: {}", message, file, line);
				break;
		}
	}
}