#pragma once
#include "Proof/Resources/Math/Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
/* REMEMBER TO IMPLEMENT SYSTEM OF NEW GET ASSET AS WE HAVE A POINTER BUT BEFORE ACCESS We have to check if ID still exist Asset*/
/* THE DESTRUCTOR OFEACH GETS CALLED WEHN THE POINTER GETS DEREFRENCED BE REMEMBER WHEN TESTING */
namespace Proof{
	class Entity;
	struct Proof_API Component{
	public:
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
	protected:
		uint32_t AssetID = 0;
		std::string Name ="Default";
		uint32_t m_EntityOwner;
		class World* CurrentWorld =nullptr;
	private:
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
	struct Proof_API TagComponent : public Component {
		TagComponent() = default;

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
		class ScriptableEntity* Instance = nullptr;
		class ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);
		template<class T,typename... Args>
		void Bind(Args... arg) {
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T(arg...)); };
			DestroyScript = [](NativeScriptComponent* NSC) {delete NSC->Instance; NSC->Instance = nullptr; };
		}
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		uint32_t StartIndexSlot;
	};

	struct Proof_API MeshComponent :public Component {
		MeshComponent() {
			MeshLocalTransform.Scale = Vector{0.0f,0.0f,0.0f};
		}
		class Model* GetModel() {
			return GetAsset() != nullptr ? GetAsset()->GetModel() : nullptr;
		}
		MeshAsset* GetAsset() {
			MeshAsset* a =(MeshAsset*)AssetManager::GetAsset(AssetID);

			if(a ==nullptr)
				AssetID =0;
			return a;
		}
		
		class Material* GetMaterial();

		uint32_t GetMaterialPointerID(){
			return m_MeshMaterialID;
		}
		bool HasMaterial(){
			return m_MeshMaterialID;
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
		glm::vec4 Colour = {1.0f,1.0f,1.0f,1.0f};
		TransformComponent SpriteTransfrom;
		Count<Texture2D> GetTexture(){
			if(GetAsset() != nullptr){
				return GetAsset()->m_Texture;
			}
			return nullptr;
		}
		Texture2DAsset* GetAsset(){
			Texture2DAsset* a = (Texture2DAsset*)AssetManager::GetAsset(AssetID);
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
}