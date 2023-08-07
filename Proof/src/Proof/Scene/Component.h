#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/Renderer/Texture.h"
#include <unordered_set>
#include "Material.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer//ParticleSystem.h"
#include "Proof/Math/Vector.h"
#include "Proof/Math/MathConvert.h"
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
		bool SetOwner(UUID newOwner) {
			m_OwnerID = newOwner; // poitn to the enitty of that child
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

		bool AddChild(UUID child) {
			if (HasChild(child) == true)return true;
			m_Children.emplace_back(child);
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

		bool RemoveChild(UUID id) {
			if (HasChildren() == false)return false;
			auto it = std::find(m_Children.begin(), m_Children.end(), id);
			if (it == m_Children.end())// checking if we have the child
				return false;
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
		Vector GetFowardVector()const;
		Vector GetRightVector()const;
		Vector GetUpVector()const;
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
		MeshComponent(const MeshComponent&other):
		m_MeshID(other.m_MeshID)
			,MaterialTable(Count<class MaterialTable>::CreateFrom(other.MaterialTable))
		{
		}

		void SetMesh(UUID ID,bool takeMaterialTable = true);
		void RemoveMesh();

		// we could do  check and compare if these material table equls the meshes material table when serilizing
		// if they do match then we just store in serilize using hte meshes
		//if no mathc then we save the material table
		Count<MaterialTable> MaterialTable = Count<class MaterialTable>::Create();
		Count<class Mesh> GetMesh();
		bool Visible = true;
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		friend class SceneRendererUI;
		friend class Editore3D;
		AssetID m_MeshID = 0;
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
		Count<Texture2D> Texture;
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		uint32_t StartIndexSlot = 0;
	};

	struct SkyLightComponent{
	public:
		SkyLightComponent(const SkyLightComponent&) = default;
		SkyLightComponent() = default;
		Vector ColorTint{1};
		float SkyBoxLoad = 0.0f;
		float MapRotation = 0.0f;
		float Intensity = 1.0f;

		void RemoveImage();
		void LoadMap(AssetID asset);
		Count<Environment> Environment;
	private:
		AssetID Image;
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
		Vector UPVector = {0,1,0};
		
		CameraType Type = CameraType::Perspective;
		float NearPlane = 0.1;
		float FarPlane = 1000;
		float FovDeg = 45;
		uint32_t Width = 250,Height = 250;
		glm::mat4 View = glm::mat4(1.0f);
		glm::mat4 Projection = glm::mat4(1.0f);
		glm::mat4 CameraMatrix = glm::mat4(1.0f);

		void CalculateProjection(const Proof::Vector& position, const Vector& rotation) {
			glm::vec3 CameraDirection;
			CameraDirection.x = cos(glm::radians(rotation.Z)) * cos(glm::radians(rotation.Y));
			CameraDirection.y = sin(glm::radians(rotation.Y));
			CameraDirection.z = sin(glm::radians(rotation.Z)) * cos(glm::radians(rotation.Y));
			Vector direction = GlmVecToProof(glm::normalize(CameraDirection));
			View = glm::lookAt(ProofToglmVec(position), ProofToglmVec(position) + ProofToglmVec(direction), ProofToglmVec(UPVector));
			Projection = glm::perspective(glm::radians(FovDeg), (float)Width / (float)Height, NearPlane, FarPlane);
			CameraMatrix = View * Projection;
		}

		bool UseLocalRotation = false;
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
		std::unordered_set<std::string> ScriptsNames;
	};

	struct TextComponent {
	public:
		TextComponent(const TextComponent& other) = default;
		TextComponent() = default;

		//chagne to u32 string in teh futre 
		std::string Text;
		glm::vec4 Colour{ 1 };

		// horizontal distnace between each character
		float Kerning = 0.0f;
		// spacing of new line 
		float LineSpacing = 1.0f;
		//Todo background color

		bool UseLocalRotation = false;
	};
	
	struct PlayerInputComponent {
	public:
		Players InputPlayer  = Players::None;
		Count<class Prefab> Player;
	};

	struct ParticleSystemComponent {
	public:
		ParticleSystemComponent(const ParticleSystemComponent& other) = default;
		ParticleSystemComponent() = default;
		Count< ParticleHandlerTable> ParticleHandlerTable = Count<class ParticleHandlerTable>::Create();
	};

	struct PlayerHUDComponent {
		Count< UITable> HudTable = Count<class UITable>::Create();
	};
	template<class ... Component>
	struct ComponentGroup {

	};
	using AllComponents =
		ComponentGroup<IDComponent, TagComponent, ChildComponent, TransformComponent,
		MeshComponent, SkyLightComponent, DirectionalLightComponent, PointLightComponent,SpotLightComponent, CameraComponent,
		CubeColliderComponent, SphereColliderComponent, CapsuleColliderComponent,MeshColliderComponent,RigidBodyComponent,
		ScriptComponent, TextComponent, PlayerInputComponent, PlayerHUDComponent, ParticleSystemComponent>;
	

	using LightComponnet =ComponentGroup<SkyLightComponent, DirectionalLightComponent, PointLightComponent, SpotLightComponent>;
}



