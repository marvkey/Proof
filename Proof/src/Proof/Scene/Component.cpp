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
	
	Material* MeshComponent::GetMaterial() {
		/*
		if (m_MeshMaterialID == 0) {
			return nullptr;
		}
		MaterialAsset* a = AssetManager::GetAsset<MaterialAsset>(m_MeshMaterialID);
		if (a == nullptr) {
			m_MeshMaterialID = 0;
			return nullptr;
		}
		return &a->m_Material;
		*/
		if (m_MeshMaterialID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<MaterialAsset>(m_MeshMaterialID);
		if (a == nullptr) {
			m_MeshMaterialID = 0;
			return nullptr;
		}
		return &a->m_Material;
	}
	void MeshComponent::SetMeshSource(AssetID ID) {
		if (ID.Get() == 0)return;
		if (AssetManager::HasID(ID)) {
			m_MeshAssetPointerID = ID;
		}
	}	
	Vector TransformComponent::GetWorldLocation()const {
		if (entID ==0 )return Location;
		Entity enttity(entID, m_World);
		if(enttity.HasOwner())
			return Location + enttity.GetOwner().GetComponent<TransformComponent>()->GetWorldLocation();
		return Location;
	}
	Vector TransformComponent::GetWorldRotation()const {
		if (entID == 0)return Rotation;
		Entity enttity(entID, m_World);
		if (enttity.HasOwner())
			return Rotation + enttity.GetOwner().GetComponent<TransformComponent>()->GetWorldRotation();
		return Rotation;
	}
	Vector TransformComponent::GetWorldScale()const {
		if (entID == 0)return Scale;
		Entity enttity(entID, m_World);
		if (enttity.HasOwner())
			return Scale + enttity.GetOwner().GetComponent<TransformComponent>()->GetWorldScale();
		return Scale;
	}
	glm::mat4 TransformComponent::GetWorldTransform() const {
		glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3{ glm::radians(Rotation.X), glm::radians(Rotation.Y), glm::radians(Rotation.Z) }));
		return glm::translate(glm::mat4(1.0f), { GetWorldLocation()}) 
			*rotation
			* glm::scale(glm::mat4(1.0f), { GetWorldScale()});
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
	
	/*

		if (neweOwner == m_CurrentID || neweOwner == m_OwnerID) {
			PF_WARN("cannot add enity as owenr of entity");
			return false;
		}
		auto it = std::find(neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.begin(), neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.end(), neweOwner);
		if (it == neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.end()) {
			if (HasEntityOwner() == true) {
				m_Owner.GetComponent<SubEntityComponet>()->RemoveSubEnity(this->GetOwner());
			}
			m_Owner = neweOwner;
			neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.emplace_back(GetOwner());
		}
		*/
	

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
		if (m_RuntimeBody == nullptr) return;
		if (m_RigidBodyType == RigidBodyType::Static)return;
		auto physicsEngine = SceneCoreClasses::GetCurrentPhysicsEngine();
		if (physicsEngine == nullptr)return;

		switch (physicsEngine->GetPhysicsType()) {
			case Proof::PhysicsEngineType::ProofPhysics:
				break;
			case Proof::PhysicsEngineType::NvdiaPhysics:
			{
				physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
				rigidBody->addForce({ force.X,force.Y,force.Z }, (physx::PxForceMode::Enum)mode, autoWake);
				break;
			}
			case Proof::PhysicsEngineType::BulletPhysics:
				break;
			default:
				break;
		}
	}

	void RigidBodyComponent::AddTorque(Vector force, ForceMode mode, bool autoWake)const {
		if (m_RuntimeBody == nullptr) return;
		if (m_RigidBodyType == RigidBodyType::Static)return;
		auto physicsEngine = SceneCoreClasses::GetCurrentPhysicsEngine();
		if (physicsEngine == nullptr)return;

		switch (physicsEngine->GetPhysicsType()) {
		case Proof::PhysicsEngineType::ProofPhysics:
			break;
		case Proof::PhysicsEngineType::NvdiaPhysics:
		{
			physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
			rigidBody->addTorque({ force.X,force.Y,force.Z }, (physx::PxForceMode::Enum)mode, autoWake);
			break;
		}
		case Proof::PhysicsEngineType::BulletPhysics:
			break;
		default:
			break;
		}
	}

	bool RigidBodyComponent::IsSleeping()const {
		if (m_RuntimeBody == nullptr) return true;
		if (m_RigidBodyType == RigidBodyType::Static)return true;
		auto physicsEngine = SceneCoreClasses::GetCurrentPhysicsEngine();
		if (physicsEngine == nullptr)return true;

		switch (physicsEngine->GetPhysicsType()) {
			case Proof::PhysicsEngineType::ProofPhysics:
				break;
			case Proof::PhysicsEngineType::NvdiaPhysics:
			{
				physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
				return rigidBody->isSleeping();
				break;
			}
			case Proof::PhysicsEngineType::BulletPhysics:
				break;
			default:
				break;
		}
		return false;
	}

	void RigidBodyComponent::PutToSleep() {
		if (m_RuntimeBody == nullptr) return;
		if (m_RigidBodyType == RigidBodyType::Static)return;
		auto physicsEngine = SceneCoreClasses::GetCurrentPhysicsEngine();
		if (physicsEngine == nullptr)return;

		switch (physicsEngine->GetPhysicsType()) {
			case Proof::PhysicsEngineType::ProofPhysics:
				break;
			case Proof::PhysicsEngineType::NvdiaPhysics:
			{
				physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
				rigidBody->putToSleep();
				break;
			}
			case Proof::PhysicsEngineType::BulletPhysics:
				break;
			default:
				break;
		}
	}

	void RigidBodyComponent::WakeUp() {
		if (m_RuntimeBody == nullptr) return;
		if (m_RigidBodyType == RigidBodyType::Static)return;
		auto physicsEngine = SceneCoreClasses::GetCurrentPhysicsEngine();
		if (physicsEngine == nullptr)return;

		switch (physicsEngine->GetPhysicsType()) {
		case Proof::PhysicsEngineType::ProofPhysics:
			break;
		case Proof::PhysicsEngineType::NvdiaPhysics:
		{
			physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
			rigidBody->wakeUp();
			break;
		}
		case Proof::PhysicsEngineType::BulletPhysics:
			break;
		default:
			break;
		}
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
