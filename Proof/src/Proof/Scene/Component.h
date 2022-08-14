#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Math/Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
#include "Proof/Renderer/Texture.h"
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
		IDComponent(const IDComponent&) 
		{

		}
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
				PF_WARN("cannot add enity as owenr of entity");
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
				PF_WARN("cannot add enity as owenr of entity");
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
		Vector<float> Location = {0.0f,0.0f,0.0f};
		Vector<float> Rotation = {0.0f,0.0f,0.0f};
		Vector<float> Scale = {1.0f,1.0f,1.0f};
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
		
		Vector<float> GetWorldLocation() const;
		Vector<float> GetWorldRotation() const;
		Vector<float> GetWorldScale() const;
		glm::mat4 GetLocalTransform() const {
			return glm::translate(glm::mat4(1.0f), { Location }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.X), { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.Y), { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.Z), { 0,0,1 })
				* glm::scale(glm::mat4(1.0f), { Scale });
		}

		glm::mat4 GetWorldTransform() const;
		Vector<float> GetFowardVector()const {
			// NOT IMPLEMENTED
			return { cos(glm::radians(Rotation.Y)) * sin(glm::radians(Rotation.Z)), sin(glm::radians(-Rotation.Y)), cos(glm::radians(Rotation.Y)) * cos(glm::radians(Rotation.Z)) };
		}
		Vector<float> GetRightVector()const {
			// NOT IMPLEMENTED

			return { cos(Rotation.Z), 0, -sin(Rotation.Z)};
		}
		Vector<float> GetUpVector()const {
			// NOT IMPLEMENTED

			return Vector<float>::Cross(GetFowardVector(), GetRightVector());
		}
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class Entity;
	private:
		uint64_t entID = 0; /// thi is to get the world transform 
		class World* m_World = nullptr;
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

	struct Proof_API MeshComponent{
		MeshComponent()=default;
		class Mesh* GetMeshSource() {
			MeshAsset* meshasset= GetAsset();
			return meshasset != nullptr ? meshasset->GetMesh() : nullptr;
		}
		MeshComponent(const MeshComponent&) = default;
	
		class Material* GetMaterial();

		uint32_t GetMaterialPointerID() {
			return m_MeshMaterialID;
		}
		bool HasMaterial() {
			return GetMaterial() == nullptr ? false : true;
		}
		UUID GetMeshAssetID();
	private:
		MeshAsset* GetAsset();
		void SetMeshSource(UUID ID);
		void RemoveMeshSource() {
			m_MeshAssetPointerID = 0;
			m_MeshAssetPointer = nullptr;
		}
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		friend class SceneRendererUI;
		friend class Editore3D;
		friend class OpenGLRenderer3DPBR;
		uint32_t m_MeshMaterialID = 0;
		UUID m_MeshAssetPointerID=0;
		Count<MeshAsset>m_MeshAssetPointer=nullptr; // MIGHT Remove
	};

	/* THIS IS TEMPORARY THIS IS GONNA GO INTO THE 2D SECTION */
	struct Proof_API SpriteComponent{
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent() = default;
		glm::vec4 Colour = {1.0f,1.0f,1.0f,1.0f};
		const Count<Texture2D>& GetTexture(){
			if (GetAsset() != nullptr) {
				return m_TextureAssetPointer->GetTexture();
			}
			return nullptr;
		}
		Texture2DAsset* GetAsset();

		void RemoveTexture(){
			m_TextureAssetPointerID = 0;
			m_TextureAssetPointer = nullptr;
		}
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		uint32_t StartIndexSlot = 0;
		UUID m_TextureAssetPointerID = 0;
		Count<Texture2DAsset> m_TextureAssetPointer;
	};

	struct Proof_API LightComponent{
		LightComponent(const LightComponent&) = default;
		LightComponent() = default;
		enum class LightType {
			Direction = 0,
			Point = 1,
			Spot = 2
		};
		float Intensity =1;
		float m_CutOff=1; // gets put in cos and radias before use
		float m_OuterCutOff=2; // gets put in cos and radias before use

		float m_Constant = 1.0f;
		float m_Linear=1;
		float m_Quadratic=1;

		float Radius=10;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		LightType m_LightType = LightType::Direction;
	};

	struct Proof_API SkyLightComponent{
		SkyLightComponent(const SkyLightComponent&) = default;
		SkyLightComponent() = default;
	private:
		//void SetHDRIPath(const std::string& path):
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
		Vector<float> m_Up = {0,1,0};
	private:
		void CalculateProjection(const Proof::Vector<float>&position, const Vector<float>& rotation) {
			glm::vec3 CameraDirection;
			CameraDirection.x = cos(glm::radians(rotation.Z)) * cos(glm::radians(rotation.Y));
			CameraDirection.y = sin(glm::radians(rotation.Y));
			CameraDirection.z = sin(glm::radians(rotation.Z)) * cos(glm::radians(rotation.Y));
			Vector<float> direction = glm::normalize(CameraDirection);
			m_View = glm::lookAt(glm::vec3{ position }, glm::vec3{ position } + glm::vec3{direction}, glm::vec3{ m_Up });
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
		Vector<float> OffsetLocation = { 0,0,0 };
		Vector<float> OffsetScale= { 1,1,1 };
		bool IsTrigger = false;

		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}

		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		class PhysicsMaterial* GetPhysicsMaterial();

	private:
		UUID m_PhysicsMaterialPointerID = 0;
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsEngine;
	};
	
	struct Proof_API SphereColliderComponent {
		SphereColliderComponent(const SphereColliderComponent&) = default;
		SphereColliderComponent() = default;
		Vector<float> OffsetLocation = { 0,0,0 };
		float Radius = 0.5f;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		class PhysicsMaterial* GetPhysicsMaterial();
	private:
		UUID m_PhysicsMaterialPointerID = 0;
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
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
		Vector<float> OffsetLocation = { 0,0,0 };
		float Radius = 0.5f;
		float Height = 2.0f;
		CapsuleDirection Direction = CapsuleDirection::Y;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		class PhysicsMaterial* GetPhysicsMaterial();
	private:
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsEngine;
		UUID m_PhysicsMaterialPointerID= 0;
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

		bool HasMesh() {
			return GetMesh() == nullptr ? false : true;
		}
		class PhysicsMaterial* GetPhysicsMaterial();
		class Mesh* GetMesh(){
			auto mesh = GetMeshAsset();
			return mesh == nullptr ? nullptr : mesh->GetMesh();
		}
		void RemoveMeshSource() {
			m_MeshAssetPointerID = 0;
		}
	private:
		class MeshAsset* GetMeshAsset();
		UUID m_MeshAssetPointerID = 0;// POINTS TO THE MESH ASSET
		void* m_RuntimeBody = nullptr;
		void* m_ConvexMeshRuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsEngine;
		UUID m_PhysicsMaterialPointerID = 0;
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

	//Rename ScriptClassFields
	struct ScriptFields {
		std::any Data;
		std::string Name;
		ProofMonoType Type = ProofMonoType::None;
	};
	//Rename ScriptClassData

	struct ScriptData {
	public:
		// GONNA CHANGE TO ID instead of string
		ScriptData(const std::string& className) {
			ClassName = className;
		}
		std::string ClassName;
		std::vector<ScriptFields> Fields;
	};
	struct ScriptComponent {
	public:
		ScriptComponent(const ScriptComponent&other) = default;
		ScriptComponent() = default;
		bool AddScript(const std::string& className);
		bool RemoveScript(const std::string& className) {
			for (int i = 0; i < m_Scripts.size(); i++) {
				if (m_Scripts[i].ClassName == className) {
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
			if (index <= m_Scripts.size()) {
				m_Scripts.erase(m_Scripts.begin() + index);
				return true;
			}
			return false;
		};
		bool HasScript(const std::string& className) {
			for (int i = 0; i < m_Scripts.size(); i++) {
				if (m_Scripts[i].ClassName == className) {
					return true;
				}
			}
			return false;
		}


		template <typename func>
		void ForEachScript(func f) {
			for (auto& val : m_Scripts)
				f(val.ClassName);
		}
	private:
		std::vector<ScriptData> m_Scripts;
		
		friend class ScriptEngine;
		friend class SceneHierachyPanel;
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
		
		Vector<bool>FreezeLocation = { false,false,false };
		Vector<bool>FreezeRotation = { false,false,false };

		RigidBodyType GetType() {
			return m_RigidBodyType;
		}

		void AddForce(Vector<float> force, ForceMode mode = ForceMode::Force,bool autoWake=true) const;
		void AddTorque(Vector<float> force, ForceMode mode = ForceMode::Force, bool autoWake = true)const;
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
		friend class PhysicsEngine;
	};
	template<class ... Component>
	struct ComponentGroup {

	};
	using AllComponents =
		ComponentGroup< TagComponent, ChildComponent, TransformComponent,
		MeshComponent, LightComponent, SkyLightComponent, CameraComponent,
		CubeColliderComponent, SphereColliderComponent, CapsuleColliderComponent,
		MeshColliderComponent, ScriptComponent, RigidBodyComponent>;
}