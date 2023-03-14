#include "Proofprch.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Proof/Asset/AssetManager.h"
#include "Material.h"
#include "script.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Material.h"
#include "script.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Physics/PhysicsEngine.h"
#include "Proof/Scripting/ScriptEngine.h"
namespace Proof
{
	static struct Material Empty;

	Count<PhysicsMaterial> CubeColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0)
		{
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		if (a == nullptr)
		{
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a;
	}
	Count<PhysicsMaterial> SphereColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a;
	}

	Count<PhysicsMaterial> CapsuleColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		if (a == nullptr)
		{
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a;
	}
	Count<PhysicsMaterial> MeshColliderComponent::GetPhysicsMaterial() {
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		if (a == nullptr)
		{
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a;
	}
	
	void RigidBodyComponent::AddForce(Vector force, ForceMode mode, bool autoWake)const {
		if (m_RigidBodyType == RigidBodyType::Static)return;
		if (m_RuntimeBody == nullptr) return;
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->addForce({ force.X,force.Y,force.Z }, (physx::PxForceMode::Enum)mode, autoWake);
	}

	void RigidBodyComponent::AddTorque(Vector force, ForceMode mode, bool autoWake)const {
		if (m_RuntimeBody == nullptr) return;
		if (m_RigidBodyType == RigidBodyType::Static)return;
		
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->addTorque({ force.X,force.Y,force.Z }, (physx::PxForceMode::Enum)mode, autoWake);
		
	}

	bool RigidBodyComponent::IsSleeping()const {
		if (m_RuntimeBody == nullptr) return true;
		if (m_RigidBodyType == RigidBodyType::Static)return true;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		return rigidBody->isSleeping();
	}

	void RigidBodyComponent::PutToSleep() {
		if (m_RuntimeBody == nullptr) return;
		if (m_RigidBodyType == RigidBodyType::Static)return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->putToSleep();
	}

	void RigidBodyComponent::WakeUp() {
		if (m_RuntimeBody == nullptr) return;
		if (m_RigidBodyType == RigidBodyType::Static)return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->wakeUp();
	}

	void MeshComponent::SetMesh(UUID ID, bool copyMaterialTable )
	{
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasAsset(ID))return;
		#endif // 

		m_MeshID = ID;
		if (copyMaterialTable)
			MaterialTable = AssetManager::GetAsset<Mesh>(m_MeshID)->GetMaterialTable()->Copy();
	}

	void MeshComponent::RemoveMesh()
	{
		m_MeshID = 0;
	}

	Count<Mesh> MeshComponent::GetMesh()
	{
		if (m_MeshID == 0)return nullptr;
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasAsset(m_MeshID)) { m_MeshID = 0; return nullptr; };
		#endif 
		return AssetManager::GetAsset<Mesh>(m_MeshID);
	}
	

	Count<Mesh> MeshColliderComponent::GetMesh()
	{
		if (m_MeshAssetPointerID == 0)return nullptr;

		
		if (AssetManager::HasAsset(m_MeshAssetPointerID))
		{
			if (PhysicsMeshCooker::HasMesh(m_MeshAssetPointerID))
			{
				return PhysicsMeshCooker::GetConvexMeshAsMesh(m_MeshAssetPointerID);
			}
			return AssetManager::GetAsset<Mesh>(m_MeshAssetPointerID);
		}
		return nullptr;
	}
}
