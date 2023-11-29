#include "Proofprch.h"
#include "PhysicsEngine.h"
#include "Proof/Asset/AssetManager.h"
#include "PhysicsDebugger.h"
#include "PhysicsMeshCache.h"
#include "MeshCollider.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Mesh.h"
namespace Proof {
	namespace Utils {
		
	}
	struct PhysXData
	{
		std::string LastErrorMessage = "";
	};
	static PhysXData* s_PhysXData;

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
	
	physx::PxDefaultCpuDispatcher* PhysicsEngine::GetCpuDispatcher()
	{
		return s_Dispatcher;
	}
	Count<MeshCollider> PhysicsEngine::GetOrCreateColliderAsset(Entity entity, MeshColliderComponent& component)
	{
		Count<MeshCollider> colliderAsset =nullptr;
		if(AssetManager::HasAsset(component.ColliderID))
			colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		if (colliderAsset)
			return colliderAsset;

		if (entity.HasComponent<DynamicMeshComponent>())
		{
			auto& mc = entity.GetComponent<DynamicMeshComponent>();
			if (mc.GetMesh() != nullptr)
			{
				component.ColliderID = AssetManager::CreateRuntimeAsset<MeshCollider>(mc.GetMesh()->GetID())->GetID();
				component.SubMeshIndex = mc.GetSubMeshIndex();
			}
		}
		else if (entity.HasComponent<MeshComponent>())
		{
			auto& mc = entity.GetComponent<MeshComponent>();
			if(mc.GetMesh()!= nullptr)
				component.ColliderID = AssetManager::CreateRuntimeAsset<MeshCollider>(mc.GetMesh()->GetID())->GetID();
		}

		if(AssetManager::HasAsset(component.ColliderID))
			colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		if (colliderAsset && !PhysicsMeshCache::Exists(colliderAsset))
			PhysicsMeshCooker::CookMesh(component.ColliderID);

		return colliderAsset;
	}
	void PhysicsEngine::Init(PhysicsSettings settings)
	{
		Timer time;
		s_PhysXData = new PhysXData();
		s_Settings = settings;

		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_DefaultAllocatorCallback, s_DefaultErrorCallback);

		PF_CORE_ASSERT(s_Foundation, "PxCreateFoundation failed!");
		s_Pvd = physx::PxCreatePvd(*s_Foundation);

		physx::PxTolerancesScale scale;
		scale.length = 1.0f;
		scale.speed = 100.0f;
		PhysicsDebugger::Initialize();

		#ifdef PF_ENABLE_DEBUG
		static bool s_TrackMemoryAllocations = true;
		#else
		static bool s_TrackMemoryAllocations = false;
		#endif

		s_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, scale, s_TrackMemoryAllocations, s_Pvd);
		PF_CORE_ASSERT(s_Physics, "PxCreatePhysics failed.");

		bool extentionsLoaded = PxInitExtensions(*s_Physics, PhysicsDebugger::GetDebugger());
		PF_CORE_ASSERT(extentionsLoaded, "Failed to initialize PhysX Extensions.");

		s_Dispatcher = physx::PxDefaultCpuDispatcherCreate(1);

		PhysicsMeshCache::Init();

		PhysicsMeshCooker::Init();
		
		PF_ENGINE_INFO("Physics Engine Initialized {}m/s", time.ElapsedMillis());
	}
	void PhysicsEngine::Release()
	{
		Timer time;
		PhysicsMeshCooker::ShutDown();
		PhysicsMeshCache::ShutDown();
		PhysicsDebugger::Shutdown();

		s_Physics->release();
		s_Dispatcher->release();
		s_Pvd->release();
		s_Foundation->release();
		delete s_PhysXData;
		PF_ENGINE_INFO("Physics Engine Shutdown {}m/s", time.ElapsedMillis());
	}
	std::string PhysicsEngine::GetLastErrorMessage()
	{
		return s_PhysXData->LastErrorMessage;
	}

	void PhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		const char* errorMessage = NULL;


		switch (code)
		{
		case physx::PxErrorCode::eNO_ERROR:				errorMessage = ""; break;
		case physx::PxErrorCode::eDEBUG_INFO:			errorMessage = "Info"; break;
		case physx::PxErrorCode::eDEBUG_WARNING:		errorMessage = "Warning"; break;
		case physx::PxErrorCode::eINVALID_PARAMETER:	errorMessage = "Invalid Parameter"; break;
		case physx::PxErrorCode::eINVALID_OPERATION:	errorMessage = "Invalid Operation"; break;
		case physx::PxErrorCode::eOUT_OF_MEMORY:		errorMessage = "Out Of Memory"; break;
		case physx::PxErrorCode::eINTERNAL_ERROR:		errorMessage = "Internal Error"; break;
		case physx::PxErrorCode::eABORT:				errorMessage = "Abort"; break;
		case physx::PxErrorCode::ePERF_WARNING:			errorMessage = "Performance Warning"; break;
		case physx::PxErrorCode::eMASK_ALL:				errorMessage = "Unknown Error"; break;
		}

		s_PhysXData->LastErrorMessage = fmt::format("{0}: {1}", errorMessage, message);
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
				PF_CORE_ASSERT(false);
				break;
			default:
				PF_EC_CRITICAL("(Physx Unkown Error) Message: {}, File: {}, Line: {}", message, file, line);
				break;
		}
	}
}