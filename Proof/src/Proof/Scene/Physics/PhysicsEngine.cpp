#include "Proofprch.h"
#include "PhysicsEngine.h"
#include "../Component.h"
#include "../World.h"
#include "../Entity.h"
#include "Proof/Asset/AssetManager.h"
#include "../Material.h"
#include "../CollisionInfo.h"
#include "../Script.h"
#include "../Mesh.h"

namespace Proof {
	namespace Utils {
		
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