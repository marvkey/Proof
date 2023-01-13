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
	

	void MeshComponent::SetMeshSource(AssetID ID) {
		if (ID.Get() == 0)return;
		if (AssetManager::HasID(ID)) {
			m_MeshAssetPointerID = ID;
		}
	}	
	

	MeshAsset* MeshComponent::GetAsset() {
		/*
		if (m_MeshAssetPointerID == 0)
			return nullptr;
		if (m_MeshAssetPointer == nullptr)
			m_MeshAssetPointer = AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID);

		if (m_MeshAssetPointer == nullptr)// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
			return nullptr;
		if (AssetManager::HasID(m_MeshAssetPointerID)) {
			return m_MeshAssetPointer.get();
		}
		m_MeshAssetPointerID = 0;
		m_MeshAssetPointer = nullptr;
		return nullptr;
		*/
		/*
		if (m_MeshAssetPointerID == 0)
			return nullptr;
		m_MeshAssetPointer = AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID); // we reasign if the asset manager returns null then the
		// shared pointer is null

		if (m_MeshAssetPointer == nullptr) {// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
			m_MeshAssetPointerID = 0;
			return nullptr;
		}
		
		return m_MeshAssetPointer.get();
		*/
		/*
		if (m_MeshAssetPointerID == 0)
			return nullptr;
		if (AssetManager::HasID(m_MeshAssetPointerID)) {
			if (m_MeshAssetPointer == nullptr)
				m_MeshAssetPointer = AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID);
			
			if(m_MeshAssetPointer->GetID() != m_MeshAssetPointerID)
				m_MeshAssetPointer = AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID);

			return m_MeshAssetPointer.get();
		}
		m_MeshAssetPointerID = 0;
		return nullptr;
		*/
		if (m_MeshAssetPointerID == 0) {
			return nullptr;
		}
		if (AssetManager::HasID(m_MeshAssetPointerID)) {
			if (AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID) != nullptr) {
				return AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID).get();
			}
			return nullptr;
		}
		m_MeshAssetPointerID == 0;
		return nullptr;
	}
	
	

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
	PhysicsMaterial* CubeColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return &a->m_Material;
	}
	PhysicsMaterial* SphereColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return &a->m_Material;
	}

	PhysicsMaterial* CapsuleColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return &a->m_Material;
	}
	PhysicsMaterial* MeshColliderComponent::GetPhysicsMaterial() {
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterialAsset>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return &a->m_Material;
	}
	
	MeshAsset* MeshColliderComponent::GetMeshAsset()const {
		if (m_MeshAssetPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<MeshAsset>(m_MeshAssetPointerID);
		if (a == nullptr) {
			m_MeshAssetPointerID = 0;
			return nullptr;
		}
		return a.get();
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

}
