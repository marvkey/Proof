#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/Renderer/Texture.h"
#include "Material.h"
#include "Proof/Scripting/MonoTypes.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include<vector>
#include <string>
#include <format>
#include <set>
#include <any>
/* REMEMBER TO IMPLEMENT SYSTEM OF NEW GET ASSET AS WE HAVE A POINTER BUT BEFORE ACCESS We have to check if ID still exist Asset*/
/* THE DESTRUCTOR OFEACH GETS CALLED WEHN THE POINTER GETS DEREFRENCED BE REMEMBER WHEN TESTING */
namespace Proof
{
	struct Proof_API IDComponent {
		const UUID& GetID() const {
			return m_ID;
		}
		IDComponent(const IDComponent&) = default;
		IDComponent() = default;

		IDComponent(UUID ID) :
			m_ID(ID)
		{

		}
	private:

		UUID m_ID;
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
	struct Proof_API TagComponent{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		/*TODO CHANGE TO SUBTAG*/
		void AddTag(const std::string& Tag) {
			m_Tags.emplace_back(Tag);
		}
		bool HasTag(const std::string& Tag)const {
			for (const std::string& TagName : m_Tags) {
				if (Tag == TagName) {
					return true;
				}
			}
			return false;
		}

		bool RemoveTag(uint32_t index){
			if(index <=m_Tags.size()){
				m_Tags.erase(m_Tags.begin()+ index);
				return true;
			}
			return false;
		};
		std::string Tag;
	private:
		std::vector<std::string> m_Tags;
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
	struct Proof_API ChildComponent { 
	public:
		ChildComponent(const ChildComponent& other) = default;
		ChildComponent() = default;
		uint64_t GetNumChildren()const { return m_Children.size(); }
		bool HasOwner()const {
			return m_OwnerID != 0;
		}
		bool HasChildren()const {
			return m_Children.size() > 0;
		}
		UUID GetOwnerID()const {
			return m_OwnerID;
		}
		const ChildComponent* GetOwner() {
			if (HasOwner() == false)return nullptr;
			return m_OwnerPointer;
		}
		bool HasChild(UUID ID) {
			return std::find(m_Children.begin(), m_Children.end(), ID.Get()) != m_Children.end();
		}
		bool HasChild(const ChildComponent& other) {
			return HasChild(other.m_CurrentID);
		}
		const std::vector<UUID>& GetChildren()const {
			return m_Children;
		}
		bool SetOwnerEmpty() {
			if (HasOwner()) {
				return m_OwnerPointer->RemoveChild(*this);
			}
			m_OwnerID = 0;
			m_OwnerPointer = nullptr;
			return true;
		}
		bool SetOwner(ChildComponent& newOwner) {
			if (newOwner == *this) {
				PF_EC_WARN("cannot add enity as owenr of entity");
				return false;
			}
			if (m_OwnerID == newOwner.m_CurrentID) // if we are already poiting to the new owenr as it already our owner
				return true;

			if (newOwner.GetOwnerID() == m_CurrentID) {// if the owner of the newowenr is htis we have to return fals
				return false;
			}

			if (HasOwner()) {
				m_OwnerPointer->RemoveChild(*this);
			}
			m_OwnerID = newOwner.m_CurrentID; // poitn to the enitty of that child
			m_OwnerPointer = &newOwner;
			newOwner.m_Children.emplace_back(m_CurrentID);
			return true;
		}

		bool AddChild(ChildComponent& child) {
			if (child == *this) {
				PF_EC_WARN("cannot add enity as owenr of entity");
				return false;
			}
			if (HasChild(child) == true)return true;
			if (child.HasOwner())
				child.SetOwnerEmpty();

			m_Children.emplace_back(child.m_CurrentID);
			child.m_OwnerID = m_CurrentID;
			child.m_OwnerPointer = this;
			return true;
		}
		bool RemoveChild(ChildComponent& child) {
			if (HasChildren() == false)return false;
			auto it = std::find(m_Children.begin(), m_Children.end(), child.m_CurrentID);
			if (it == m_Children.end())// checking if we have the child
				return false;
			child.m_OwnerID = 0;
			child.m_OwnerPointer = nullptr;// we should not rely on this because an entity can be deleted and the pointer could
			// still be poiting to that empty memeoory block
			m_Children.erase(it);
			return true;
		}
		bool operator==(const ChildComponent& other)const {
			return other.m_CurrentID == m_CurrentID && other.m_OwnerID == m_OwnerID;
		}
		bool operator!=(const ChildComponent& other) const {
			return !(*this == other);
		}

		int GetChildIndex(ChildComponent& child)const {
			for (int i = 0; i < m_Children.size(); i++) {
				if (m_Children[i].Get() == child.m_CurrentID.Get())return i;
			}
			return -1;
		}
	private:

		UUID m_CurrentID = 0; // entity its attached to ID
		UUID m_OwnerID = 0; // owner of the entity its attahced to
		std::vector<UUID>m_Children;
		ChildComponent* m_OwnerPointer = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class Entity;
		friend class Editore3D;
	};
	struct Proof_API TransformComponent {
		Vector Location = {0.0f,0.0f,0.0f};
		Vector Rotation = {0.0f,0.0f,0.0f};
		Vector Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) {
			Location = other.Location;
			Rotation = other.Rotation;
			Scale = other.Scale;
		}
		TransformComponent operator+ (const TransformComponent& other)const {
			TransformComponent temp;
			temp.Location = this->Location + other.Location;
			temp.Rotation = this->Rotation + other.Rotation;
			temp.Scale = this->Scale + other.Scale;
			return temp;
		}
		
		glm::mat4 GetLocalTransform() const {
			return glm::translate(glm::mat4(1.0f), ProofToglmVec(Location)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.X), { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.Y), { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.Z), { 0,0,1 })
				* glm::scale(glm::mat4(1.0f), ProofToglmVec(Scale));
		}
		Vector GetFowardVector()const {
			// NOT IMPLEMENTED
			return { cos(glm::radians(Rotation.Y)) * sin(glm::radians(Rotation.Z)), sin(glm::radians(-Rotation.Y)), cos(glm::radians(Rotation.Y)) * cos(glm::radians(Rotation.Z)) };
		}
		Vector GetRightVector()const {
			// NOT IMPLEMENTED

			return { cos(Rotation.Z), 0, -sin(Rotation.Z)};
		}
		Vector GetUpVector()const {
			// NOT IMPLEMENTED
			return GetFowardVector().Cross(GetRightVector());
		}
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class Entity;
	};
	
	struct Proof_API NativeScriptComponent{
		NativeScriptComponent(const NativeScriptComponent& other) {
			this->Instance = other.Instance;
			this->InstantiateScript = other.InstantiateScript;
			this->DestroyScript = other.DestroyScript;
			this->m_ScriptPointerName = other.m_ScriptPointerName;
			this->m_HasScriptAttached = other.m_HasScriptAttached;
		}
		NativeScriptComponent() = default;
		class Script* Instance = nullptr;
		class Script* (*InstantiateScript)()=nullptr;
		void (*DestroyScript)(NativeScriptComponent*)=nullptr;

		template<class T,typename... Args>
		void Bind(Args... arg) {
			m_HasScriptAttached = true;
			InstantiateScript = []() { return static_cast<Script*>(new T(arg...)); };
			DestroyScript = [](NativeScriptComponent* NSC) {delete NSC->Instance; NSC->Instance = nullptr;};
		}

		const std::string& GetScriptName()const {
			return m_ScriptPointerName;
		}
	private:
		std::string m_ScriptPointerName = "null";
		friend class SceneHierachyPanel;
		friend class World;
		bool m_HasScriptAttached = false;
	};

	struct MeshComponent{
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;

		bool HasMaterial();
		AssetID GetMaterialID()const;
		void SetMaterial(AssetID ID);
		void RemoveMaterial();

		AssetID GetMeshID()const;
		void SetMesh(UUID ID);
		void RemoveMesh();

		Count<class Mesh> GetMesh();
		Count<class Material> GetMaterial();
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		friend class SceneRendererUI;
		friend class Editore3D;
		AssetID m_MeshID = 0;
		AssetID m_MaterialID=0;
	};

	struct DynamicMeshComponent {
	public:

	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		friend class SceneRendererUI;
		friend class Editore3D;
		UUID m_MeshID;
		uint32_t m_SubmeshIndex = 0;
	};
	struct Proof_API SpriteComponent{
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent() = default;
		glm::vec4 Colour = {1.0f,1.0f,1.0f,1.0f};

		void RemoveTexture(){
			m_TextureAssetPointerID = 0;
		}
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		uint32_t StartIndexSlot = 0;
		UUID m_TextureAssetPointerID = 0;
	};

	struct SkyLightComponent{
	public:
		SkyLightComponent(const SkyLightComponent&) = default;
		SkyLightComponent() = default;
		float BoxLoad = 1.0f;;
		Vector OffsetDirection;
		Vector Color{1};
		float Intensity = 1;
		float Exposure = 1.f;
		float MapRotation = 0;
	private:
		void Generate();
		std::filesystem::path m_HDRPath;
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};

	struct DirectionalLightComponent {
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent() = default;
		float Intensity = 1;

		Vector Color = { 1 };
		Vector OffsetDirection;
	};

	struct PointLightComponent {
		PointLightComponent(const PointLightComponent&) = default;

		PointLightComponent() = default;
		Vector Color = { 1 };
		
		float Intensity = 1;
		float Constant = 1;
		float Linear = 1;
		float Quadratic = 1;
		float Radius = 10;
	};

	struct SpotLightComponent {
		SpotLightComponent(const SpotLightComponent&) = default;

		
		SpotLightComponent() = default;

		
		Vector Color = { 1 };
		
		float Intensity = 1;
		float Constant = 1;
		float Linear = 1;
		float Quadratic = 1;
		float Radius = 10;
		float CutOff = 1; // gets put in cos and radias before use
		float OuterCutOff = 2; // gets put in cos and radias before use
	};



	struct Proof_API CameraComponent{
	public:
		CameraComponent(const CameraComponent&) = default;

		CameraComponent() = default;
		enum class CameraType { Orthographic = 0,Perspective = 1 };

		void SetDimensions(uint32_t width,uint32_t Height) {
			m_Width = width;
			m_Height = Height;
		}
		bool AutoSetDimension(bool value) {
			value = m_AutoSetDimension;
		}
		const glm::mat4& GetView()const { return m_View; }
		const glm::mat4& GetProjection()const { return m_Projection; }
		Vector m_Up = {0,1,0};
	private:
		void CalculateProjection(const Proof::Vector&position, const Vector& rotation) {
			glm::vec3 CameraDirection;
			CameraDirection.x = cos(glm::radians(rotation.Z)) * cos(glm::radians(rotation.Y));
			CameraDirection.y = sin(glm::radians(rotation.Y));
			CameraDirection.z = sin(glm::radians(rotation.Z)) * cos(glm::radians(rotation.Y));
			Vector direction = GlmVecToProof(glm::normalize(CameraDirection));
			m_View = glm::lookAt(ProofToglmVec(position), ProofToglmVec( position) + ProofToglmVec(direction), ProofToglmVec( m_Up ));
			m_Projection = glm::perspective(glm::radians(m_FovDeg),(float)m_Width / (float)m_Height,m_NearPlane,m_FarPlane);
			m_CameraMatrix = m_View * m_Projection;
		}
		CameraType m_CameraType = CameraType::Perspective;

		bool m_AutoSetDimension = true;
		float m_NearPlane = 0.1;
		float m_FarPlane = 1000;
		float m_FovDeg = 45;
		uint32_t m_Width = 250,m_Height = 250;
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_CameraMatrix = glm::mat4(1.0f);
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
	};

	struct Proof_API CubeColliderComponent {
		CubeColliderComponent(const CubeColliderComponent&) = default;
		CubeColliderComponent() = default;
		Vector OffsetLocation = { 0,0,0 };
		Vector OffsetScale= { 1,1,1 };
		bool IsTrigger = false;

		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}

		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();

	private:
		mutable UUID m_PhysicsMaterialPointerID = 0;
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsEngine;
		friend class PhysicsActor;
	};
	
	struct Proof_API SphereColliderComponent {
		SphereColliderComponent(const SphereColliderComponent&) = default;
		SphereColliderComponent() = default;
		Vector OffsetLocation = { 0,0,0 };
		float Radius = 1.0f;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();
	private:
		UUID m_PhysicsMaterialPointerID = 0;
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class PhysicsActor;
		friend class WorldRenderer;
		friend class PhysicsEngine;
	};
	enum class CapsuleDirection {
		X=1,
		Y = 0,
		Z =2
	};
	struct Proof_API CapsuleColliderComponent {
		CapsuleColliderComponent(const CapsuleColliderComponent&) = default;
		CapsuleColliderComponent() = default;
		Vector OffsetLocation = { 0,0,0 };
		float Radius = 0.5f;
		float Height = 2.0f;
		CapsuleDirection Direction = CapsuleDirection::Y;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial(){
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();
	private:
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class PhysicsActor;
		friend class WorldRenderer;
		friend class PhysicsEngine;
		mutable UUID m_PhysicsMaterialPointerID= 0;
	};

	struct Proof_API MeshColliderComponent {
		MeshColliderComponent(const MeshColliderComponent&) = default;
		MeshColliderComponent() = default;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();
		Count<class Mesh> GetMesh();

		UUID GetMeshSource() {
			return m_MeshAssetPointerID;
		}
		void RemoveMeshSource() {
			m_MeshAssetPointerID = 0;
		}
	private:
		mutable UUID m_MeshAssetPointerID = 0;// POINTS TO THE MESH ASSET
		void* m_RuntimeBody = nullptr;
		void* m_ConvexMeshRuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsActor;
		friend class PhysicsEngine;
		mutable UUID m_PhysicsMaterialPointerID = 0;
	};	
	enum class RigidBodyType {
		Static,
		Dynamic
	};
	enum class ForceMode {
		Force,				
		Impule,			
		VelocityChange,	
		Acceleration
	};
	
	class Proof_API RigidBodyComponent {
	public:
		RigidBodyComponent(const RigidBodyComponent&) = default;
		RigidBodyComponent() = default;
		
		float Mass = 1.0f;
		float AngularDrag = 0.05f;
		float LinearDrag = 0.0f;
		bool Gravity = true;
		bool Kinimatic = false;
		
		VectorTemplate<bool>FreezeLocation = { false,false,false };
		VectorTemplate<bool>FreezeRotation = { false,false,false };

		RigidBodyType GetType() {
			return m_RigidBodyType;
		}

		void AddForce(Vector force, ForceMode mode = ForceMode::Force,bool autoWake=true) const;
		void AddTorque(Vector force, ForceMode mode = ForceMode::Force, bool autoWake = true)const;
		bool IsSleeping() const;
		void PutToSleep();
		void WakeUp();

	private:
		RigidBodyType m_RigidBodyType = RigidBodyType::Static;
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsActor;
	};

	struct ScriptComponent {
	public:
		ScriptComponent(const ScriptComponent& other) = default;
		ScriptComponent() = default;

		bool AddScript(const std::string& className);
		bool RemoveScript(const std::string& className) {
			for (int i = 0; i < m_Scripts.size(); i++)
			{
				if (m_Scripts[i].ClassName == className)
				{
					m_Scripts.erase(m_Scripts.begin() + i);
					return true;
				}
			}
			return false;
		}

		bool HasAnyScripts() {
			return m_Scripts.size() > 0;
		}
		bool RemoveScript(uint32_t index) {
			if (index <= m_Scripts.size())
			{
				m_Scripts.erase(m_Scripts.begin() + index);
				return true;
			}
			return false;
		};

		bool ChangeScript(const std::string& oldClassName, const std::string& newClassName);

		bool HasScript(const std::string& className) {
			for (int i = 0; i < m_Scripts.size(); i++)
			{
				if (m_Scripts[i].ClassName == className)
				{
					return true;
				}
			}
			return false;
		}


		template <typename func>
		void ForEachScript(func f) {
			for (const auto& val : m_Scripts)
				f(val.ClassName);
		}
	private:
		std::vector<ScriptData> m_Scripts;

		friend class ScriptEngine;
		friend class SceneHierachyPanel;
		friend class World;
		friend class SceneSerializer;
		friend class WorldRenderer;
		friend class Editore3D;
		friend class PhysicsEngine;
	};
	template<class ... Component>
	struct ComponentGroup {

	};
	using AllComponents =
		ComponentGroup<IDComponent, TagComponent, ChildComponent, TransformComponent,
		MeshComponent, DirectionalLightComponent, PointLightComponent,SpotLightComponent, CameraComponent,
		CubeColliderComponent, SphereColliderComponent, CapsuleColliderComponent,MeshColliderComponent,RigidBodyComponent,
		ScriptComponent>;
	

	using LightComponnet =ComponentGroup< DirectionalLightComponent, PointLightComponent, SpotLightComponent>;
}