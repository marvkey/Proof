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
/* REMEMBER TO IMPLEMENT SYSTEM OF NEW GET ASSET AS WE HAVE A POINTER BUT BEFORE ACCESS We have to check if ID still exist Asset*/
/* THE DESTRUCTOR OFEACH GETS CALLED WEHN THE POINTER GETS DEREFRENCED BE REMEMBER WHEN TESTING */
namespace Proof
{
	using AssetID =uint64_t;
	using EntityID = uint64_t;
	class Entity;
	struct Proof_API Component {
	public:
		Component(const Component&) = default;
		Component() = default;
		virtual ~Component() {

		}
		const std::string& GetName()const { return Name; }
		virtual void SetName(const std::string& name) {
			Name = name;
		}
		class Entity GetOwner()const;

		void Componet() {};
	protected:
		std::string Name = "Default";
		EntityID m_EntityOwner;
		class World* CurrentWorld = nullptr;
	private:
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};


	struct Proof_API TagComponent: public Component {
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		void AddTag(const std::string& Tag) {
			m_Tags.emplace_back(Tag);
			//Name = "Empty Entity";
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
	private:
		std::vector<std::string> m_Tags;
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};

	struct Proof_API TransformComponent:public Component {
		mutable Vector Location = {0.0f,0.0f,0.0f};
		mutable Vector Rotation = {0.0f,0.0f,0.0f};
		mutable Vector Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		Vector GetWorldLocation()const;
		Vector GetWorldRotation()const;
		Vector GetWorldScale()const;
		glm::mat4 GetLocalTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f),{Location})
				* rotation
				* glm::scale(glm::mat4(1.0f),{Scale});
		}

		glm::mat4 GetWorldTransform() const;
	};

	struct Proof_API NativeScriptComponent:public Component {
		NativeScriptComponent() {
			Name = "NativeScriptComponent";
		}
		NativeScriptComponent(const NativeScriptComponent&) = default;
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

	struct Proof_API MeshComponent:public Component {
		MeshComponent()=default;
		class Mesh* GetMesh() {
			return GetAsset() != nullptr ? GetAsset()->GetMesh() : nullptr;
		}
		MeshComponent(const MeshComponent&) = default;
		MeshAsset* GetAsset() {
			if(m_MeshAssetPointerID==0)
				return nullptr;
			if(m_MeshAssetPointer ==nullptr)
				m_MeshAssetPointer = AssetManager::GetAssetShared<MeshAsset>(m_MeshAssetPointerID);

			if(m_MeshAssetPointer==nullptr)// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
				return nullptr;
			if(AssetManager::HasID(m_MeshAssetPointerID)){
				return m_MeshAssetPointer.get();
			}
			m_MeshAssetPointerID=0;
			m_MeshAssetPointer =nullptr;
			return nullptr;
		}

		class Material* GetMaterial();

		uint32_t GetMaterialPointerID() {
			return m_MeshMaterialID;
		}
		bool HasMaterial() {
			return GetMaterial() == nullptr ? false : true;
		}
		uint32_t GetMeshPointerID();
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		friend class SceneRendererUI;
		uint32_t StartIndexSlot = 0;
		uint32_t m_MeshMaterialID = 0;
		AssetID m_MeshAssetPointerID=0;
		Count<MeshAsset>m_MeshAssetPointer=nullptr;
	};

	/* THIS IS TEMPORARY THIS IS GONNA GO INTO THE 2D SECTION */
	struct Proof_API SpriteComponent:public Component {
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
		AssetID m_TextureAssetPointerID;
		Count<Texture2DAsset> m_TextureAssetPointer;
	};

	struct Proof_API LightComponent: public Component {
		LightComponent(const LightComponent&) = default;
		LightComponent() = default;
		enum LightType:int {
			Direction = 0,
			Point = 1,
			Spot = 2
		};
		float m_CutOff; // gets put in cos and radias before use
		float m_OuterCutOff; // gets put in cos and radias before use

		float m_Constant = 1.0f;
		float m_Linear;
		float m_Quadratic;

		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		int m_LightType = 0;
	private:
		uint32_t StartIndexSlot = 0;
		friend class ECS;
	};
	struct Proof_API SkyLightComponent: public Component {
		SkyLightComponent(const SkyLightComponent&) = default;
		SkyLightComponent() = default;
	private:
		//void SetHDRIPath(const std::string& path):
	};


	struct Proof_API CameraComponent: public Component {
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
		Vector m_Up = {0,1,0};
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
		Vector* m_Positon = nullptr;
		Vector* m_Roatation = nullptr;

		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_CameraMatrix = glm::mat4(1.0f);
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
	private:
		uint32_t StartIndexSlot = 0;
		friend class ECS;
	};

}