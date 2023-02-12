#include "Proofprch.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Proof/Asset/MaterialAsset.h"
#include "Proof/Asset/AssetManager.h"
#include "Material.h"
#include "script.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Proof/Asset/MaterialAsset.h"
#include "Material.h"
#include "script.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Physics/PhysicsEngine.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Asset/PhysicsMaterialAsset.h"
#include "Proof/Asset/MeshAsset.h"
namespace Proof
{
	static struct Material Empty;

	Texture2DAsset* SpriteComponent::GetAsset()
	{
		if (m_TextureAssetPointerID == 0)
			return nullptr;
		if (m_TextureAssetPointer == nullptr)
			m_TextureAssetPointer = AssetManager::GetAsset<Texture2DAsset>(m_TextureAssetPointerID);

		if (m_TextureAssetPointer == nullptr)// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
			return nullptr;
		if (AssetManager::HasID(m_TextureAssetPointerID)) {
			return m_TextureAssetPointer.get();
		}
		m_TextureAssetPointerID = 0;
		m_TextureAssetPointer = nullptr;
		return nullptr;
	}
	Count<PhysicsMaterial> CubeColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a->GetMaterial();
	}
	Count<PhysicsMaterial> SphereColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a->GetMaterial();
	}

	Count<PhysicsMaterial> CapsuleColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a->GetMaterial();
	}
	Count<PhysicsMaterial> MeshColliderComponent::GetPhysicsMaterial() {
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a->GetMaterial();
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

	bool ScriptComponent::AddScript(const std::string& className) {
		if (ScriptEngine::EntityClassExists(className) == false)return false;
		if (HasScript(className) == true)return false;
		ScriptData scriptData{ className };
		const ScriptClass* scriptClass = ScriptEngine::GetScriptClass(className);
		if (scriptClass != nullptr) {
			for (auto& data : scriptClass->m_FieldData) {
				ScriptField field;
				field.Name = data.Name;
				field.Type = data.Type;
				field.Data = data.Data;
				scriptData.Fields.emplace_back(field);
			}
		}
		m_Scripts.emplace_back(scriptData);
		return true;
	}
	bool ScriptComponent::ChangeScript(const std::string& oldClassName, const std::string& newClassName) {
		if (ScriptEngine::EntityClassExists(newClassName) == false)return false;
		if (HasScript(oldClassName) == true)return false;
		int posIndex = -1;
		for (int i = 0; i < m_Scripts.size(); i++) {
			if (m_Scripts[i].ClassName == oldClassName)
				posIndex = i;
		}
		if (posIndex == -1)return false; // script not found
		ScriptData scriptData{ newClassName };
		const ScriptClass* scriptClass = ScriptEngine::GetScriptClass(newClassName);
		if (scriptClass != nullptr) {
			for (auto& data : scriptClass->m_FieldData) {
				ScriptField field;
				field.Name = data.Name;
				field.Type = data.Type;
				field.Data = data.Data;
				scriptData.Fields.emplace_back(field);
			}
		}
		// changing the data located in that script
		m_Scripts[posIndex] = scriptData;
	}

	

	AssetID MeshComponent::GetMeshID() const
	{
		return m_MeshID;
	}

	void MeshComponent::SetMesh(UUID ID)
	{
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasID(ID))return;
		#endif // 

		m_MeshID = ID;
	}

	void MeshComponent::RemoveMesh()
	{
		m_MeshID = 0;
	}

	Count<Mesh> MeshComponent::GetMesh()
	{
		if (m_MeshID == 0)return nullptr;
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasID(m_MeshID)) { m_MeshID = 0; return nullptr; };
		#endif 
		return AssetManager::GetAsset<MeshAsset>(m_MeshID)->GetMesh();
	}

	bool MeshComponent::HasMaterial()
	{
		return m_MaterialID != 0;
	}
	Count<Material> MeshComponent::GetMaterial()
	{
		if (m_MaterialID == 0)return nullptr;
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasID(m_MaterialID)) { m_MaterialID = 0; return nullptr; };
		#endif 
		return AssetManager::GetAsset<MaterialAsset>(m_MaterialID)->GetMaterial();
	}

	AssetID MeshComponent::GetMaterialID() const
	{
		return m_MaterialID;
	}

	void MeshComponent::SetMaterial(AssetID ID)
	{
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasID(ID))return;
		#endif // 

		m_MaterialID = ID;
	}

	void MeshComponent::RemoveMaterial()
	{
		m_MaterialID = 0;
	}

	Count<Mesh> MeshColliderComponent::GetMesh()
	{
		if (m_MeshAssetPointerID == 0)return nullptr;

		
		if (AssetManager::HasID(m_MeshAssetPointerID))
		{
			if (PhysicsMeshCooker::HasMesh(m_MeshAssetPointerID))
			{
				return PhysicsMeshCooker::GetConvexMeshAsMesh(m_MeshAssetPointerID);
			}
			return AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID)->GetMesh();
		}
		return nullptr;
	}
}
