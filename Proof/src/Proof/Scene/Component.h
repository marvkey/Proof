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
#include "Proof/Scene/Camera/PerspectiveCamera.h"
#include "Proof/Scene/Camera/Camera.h"
/* REMEMBER TO IMPLEMENT SYSTEM OF NEW GET ASSET AS WE HAVE A POINTER BUT BEFORE ACCESS We have to check if ID still exist Asset*/
/* THE DESTRUCTOR OFEACH GETS CALLED WEHN THE POINTER GETS DEREFRENCED BE REMEMBER WHEN TESTING */
namespace Proof{
	using EntityID = uint64_t;
	class Entity;
	struct Proof_API Component{
	public:
		Component(const Component&) = default;
		Component()=default;
		virtual ~Component(){

		}
		std::string GetName(){return Name;}
		virtual void SetName(const std::string& name){
			Name = name;
		}
		class Entity GetOwner();
		uint32_t GetAssetID() {
			return AssetID;
		}

		void Componet(){};
	protected:
		uint32_t AssetID = 0;
		std::string Name ="Default";
		EntityID m_EntityOwner;
		class World* CurrentWorld =nullptr;
	private:
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
	struct Proof_API TagComponent : public Component {
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		void AddTag(const std::string& Tag) {
			Tags.emplace_back(Tag);
			Name ="Empty Entity";
		}
		bool HasTag(const std::string& Tag) {
			for (const std::string& TagName : Tags) {
				if (Tag == TagName) {
					return true;
				}
			}
			return false;
		}
	private:
		std::vector<std::string> Tags;
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
	};

	struct Proof_API TransformComponent :public Component{
		Vector Location = {0.0f,0.0f,0.0f};
		Vector Rotation = {0.0f,0.0f,0.0f};
		Vector Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const 		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f),{Location})
				* rotation
				* glm::scale(glm::mat4(1.0f),{Scale});
		}
	};

	struct Proof_API NativeScriptComponent :public Component {
		NativeScriptComponent() {
			Name = "NativeScriptComponent";
		}
		NativeScriptComponent(const NativeScriptComponent&)=default;
		class Script* Instance = nullptr;
		class Script* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);
		
		template<class T,typename... Args>
		void Bind(Args... arg) {
			m_HasScriptAttached = true;
			InstantiateScript = []() { return static_cast<Script*>(new T(arg...));};
			DestroyScript = [](NativeScriptComponent* NSC) {delete NSC->Instance; NSC->Instance = nullptr;};
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


	struct Proof_API MeshComponent :public Component {
		MeshComponent() {
			MeshLocalTransform.Scale = Vector{0.0f,0.0f,0.0f};
		}
		class Mesh* GetMesh() {
			return GetAsset() != nullptr ? GetAsset()->GetMesh() : nullptr;
		}
		MeshComponent(const MeshComponent&)=default;
		MeshAsset* GetAsset() {
			MeshAsset* a =AssetManager::GetAsset<MeshAsset>(AssetID);

			if(a ==nullptr)
				AssetID =0;
			return a;
		}
		
		class Material* GetMaterial();

		uint32_t GetMaterialPointerID(){
			return m_MeshMaterialID;
		}
		bool HasMaterial(){
			return GetMaterial() ==nullptr? false: true;
		}
		uint32_t GetMeshPointerID();
		TransformComponent MeshLocalTransform;
		
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		uint32_t StartIndexSlot = 0;
		uint32_t m_MeshMaterialID= 0;
	};

	/* THIS IS TEMPORARY THIS IS GONNA GO INTO THE 2D SECTION */
	struct Proof_API SpriteComponent:public Component {
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent() = default;

		glm::vec4 Colour = {1.0f,1.0f,1.0f,1.0f};
		TransformComponent SpriteTransfrom;
		Count<Texture2D> GetTexture(){
			if(GetAsset() != nullptr){
				return GetAsset()->m_Texture;
			}
			return nullptr;
		}
		Texture2DAsset* GetAsset(){
			Texture2DAsset* a = AssetManager::GetAsset<Texture2DAsset>(AssetID);
			if (a == nullptr) {
				AssetID = 0;
				return nullptr;
			}
			return a;
		}
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		uint32_t StartIndexSlot =0;
	};

	struct Proof_API LightComponent: public Component{
		LightComponent(const LightComponent&) = default;
		LightComponent() = default;
		enum LightType:int{
			Direction=0,
			Point=1,
			Spot=2
		};
		Vector m_Position;
		Vector m_Direction = {1.0,1.0,1.0};
		float m_CutOff; // gets put in cos and radias before use
		float m_OuterCutOff; // gets put in cos and radias before use

		float m_Constant=1.0f;
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

	
	struct Proof_API CameraComponent:Component{
		Camera m_Camera;
	private:
		uint32_t StartIndexSlot = 0;
		friend class ECS;
	};
	
}