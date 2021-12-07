#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Math/Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
#include "Proof/Renderer/Texture.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include<vector>
#include <string>
#include <format>
#include <set>
/* REMEMBER TO IMPLEMENT SYSTEM OF NEW GET ASSET AS WE HAVE A POINTER BUT BEFORE ACCESS We have to check if ID still exist Asset*/
/* THE DESTRUCTOR OFEACH GETS CALLED WEHN THE POINTER GETS DEREFRENCED BE REMEMBER WHEN TESTING */
namespace Proof
{
	
	struct Proof_API TagComponent{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		void AddTag(const std::string& Tag) {
			m_Tags.emplace_back(Tag);
			m_Tags.size();
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

	struct Proof_API TransformComponent {
		Vector<float> Location = {0.0f,0.0f,0.0f};
		Vector<float> Rotation = {0.0f,0.0f,0.0f};
		Vector<float> Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent operator+ (const TransformComponent& other)const {
			TransformComponent temp;
			temp.Location = this->Location + other.Location;
			temp.Rotation = this->Rotation + other.Rotation;
			temp.Scale = this->Scale + other.Scale;
			return temp;
		}
		Vector<float> GetWorldLocation()const;
		Vector<float> GetWorldRotation()const;
		Vector<float> GetWorldScale()const;
		glm::mat4 GetLocalTransform() const {
			return glm::translate(glm::mat4(1.0f),{Location}) *
				glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.X),{1,0,0})
				* glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.Y),{0,1,0})
				* glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.Z),{0,0,1})
				* glm::scale(glm::mat4(1.0f),{Scale});
		}

		glm::mat4 GetWorldTransform() const;
	};
	class Proof_API ChildComponent { /* THIS CLASS KNOWS AN ENTITY EXIST */
	public:
		ChildComponent(const ChildComponent&) = default;
		ChildComponent() = default;
		bool HasOwner()const {
			return m_OwnerID !=0;
		}
		bool HasChildren()const {
			return m_Children.size() > 0;
		}
		UUID GetOwnerID()const {
			return m_OwnerID;
		}
		const ChildComponent* GetOwner(){
			if (HasOwner() == false)return nullptr;
			return m_OwnerPointer;
		}
		bool HasChild(UUID ID) {
			return m_Children.find(ID) != m_Children.end();
		}
		bool HasChild(const ChildComponent& other) {
			return HasChild(other.m_CurrentID);
		}
		const std::set<UUID>& GetChildren()const {
			return m_Children;
		}
		bool SetOwnerEmpty() {
			if (HasOwner()) {
				m_OwnerPointer->RemoveChild(*this);
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
			newOwner.m_Children.insert(m_CurrentID);
			m_OwnerPointer = &newOwner;
			return true;
		}
	
		bool AddChild(ChildComponent& child) {
			if (child == *this) {
				PF_WARN("cannot add enity as owenr of entity");
				return false;
			}
			if (HasChild(child) == true)return true;

			m_Children.insert(child.m_CurrentID);
			child.m_OwnerID = m_CurrentID;
			return true;
		}
		bool RemoveChild(ChildComponent& child) {
			if (HasChild(child) == false)
				return false;

			m_Children.erase(child.m_CurrentID);

			child.m_OwnerID = 0;
			child.m_OwnerPointer = nullptr;// we should not rely on this because an entity can be deleted and the pointer could
			// still be poiting to that empty memeoory block
			return true;
		}
		bool operator==(const ChildComponent& other)const {
			return other.m_CurrentID == m_CurrentID && other.m_OwnerID == m_OwnerID;
		}
		bool operator!=(const ChildComponent& other) const {
			return !(*this == other);
		}
	private:
		
		UUID m_CurrentID = 0; // entity its attached to ID
		UUID m_OwnerID = 0; // owner of the entity its attahced to
		std::set<UUID>m_Children;
		ChildComponent* m_OwnerPointer;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class Entity;
	};
	struct Proof_API NativeScriptComponent{
		NativeScriptComponent(const NativeScriptComponent&) = default;
		NativeScriptComponent() = default;
		class Script* Instance = nullptr;
		class Script* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<class T,typename... Args>
		void Bind(Args... arg) {
			m_HasScriptAttached = true;
			InstantiateScript = []() { return static_cast<Script*>(new T(arg...)); };
			DestroyScript = [](NativeScriptComponent* NSC) {delete NSC->Instance; NSC->Instance = nullptr; };
		}

		const std::string GetScriptName() {
			return m_ScriptPointerName;
		}
	private:
		std::string m_ScriptPointerName = "null";
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		uint32_t StartIndexSlot;
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
		uint32_t StartIndexSlot = 0;
		uint32_t m_MeshMaterialID = 0;
		UUID m_MeshAssetPointerID=0;
		Count<MeshAsset>m_MeshAssetPointer=nullptr;
	};

	/* THIS IS TEMPORARY THIS IS GONNA GO INTO THE 2D SECTION */
	struct Proof_API SpriteComponent{
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent() = default;

		glm::vec4 Colour = {1.0f,1.0f,1.0f,1.0f};
		TransformComponent SpriteTransfrom;
		const Count<Texture2D>& GetTexture(){
			if (GetAsset() != nullptr) {
				return m_TextureAssetPointer->GetTexture();
			}
			return nullptr;
		}
		 Texture2DAsset* GetAsset(){
			if (m_TextureAssetPointerID == 0)
				return nullptr;
			if (m_TextureAssetPointer == nullptr)
				m_TextureAssetPointer = AssetManager::GetAssetShared<Texture2DAsset>(m_TextureAssetPointerID);

			if (m_TextureAssetPointer == nullptr)// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
				return nullptr;
			if (AssetManager::HasID(m_TextureAssetPointerID)) {
				return m_TextureAssetPointer.get();
			}
			m_TextureAssetPointerID = 0;
			m_TextureAssetPointer = nullptr;
			return nullptr;
		}

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
		enum LightType:int {
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
		int m_LightType = 0;
	private:
		uint32_t StartIndexSlot = 0;
		friend class ECS;
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
			CalculateProjection();
		}
		bool AutoSetDimension(bool value) {
			value = m_AutoSetDimension;
		}
		const glm::mat4& GetView()const { return m_View; }
		const glm::mat4& GetProjection()const { return m_Projection; }
		Vector<float> m_Up = {0,1,0};
	private:
		void CalculateProjection() {
			if (m_Positon == nullptr || m_Roatation == nullptr)
				return;
			m_View = glm::lookAt(glm::vec3{*m_Positon},glm::vec3{*m_Positon} + glm::vec3{*m_Roatation},glm::vec3{m_Up});
			m_Projection = glm::perspective(glm::radians(m_FovDeg),(float)m_Width / (float)m_Height,m_NearPlane,m_FarPlane);
			m_CameraMatrix = m_View * m_Projection;
		}
		CameraType m_CameraType = CameraType::Perspective;

		bool m_AutoSetDimension = true;
		float m_NearPlane = 0.1;
		float m_FarPlane = 1000;
		float m_FovDeg = 45;
		uint32_t m_Width = 250,m_Height = 250;
		Vector<float>* m_Positon = nullptr;
		Vector<float>* m_Roatation = nullptr;

		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_CameraMatrix = glm::mat4(1.0f);
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
	private:
	};
		
	struct Proof_API CubeColliderComponent {
		CubeColliderComponent(const CubeColliderComponent&) = default;
		CubeColliderComponent() { Offset.Scale= Vector<float>{ 0, 0, 0 }; }
		static class Mesh* GetMeshSource();
		TransformComponent Offset;
	};

}