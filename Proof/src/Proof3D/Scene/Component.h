#pragma once
#include "Proof3D/Math/Vector.h"
#include "Proof3D/Math/Rotate.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Entity.h"

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
		Entity GetOwner();
	protected:
		std::string Name ="Default";
		Entity m_EntityOwner;
	private:
		friend class Entity;
		friend class World;
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
			PF_ENGINE_INFO("create Mesh");
			MeshLocalTransform.Scale = Vector{0.0f,0.0f,0.0f};
		}
		class Model* GetModel() {
			return m_Mesh;
		}
		class Model* m_Mesh = nullptr;
		uint32_t GetID();
		TransformComponent MeshLocalTransform;
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		uint32_t StartIndexSlot;
	};

	/* THIS IS TEMPORARY THIS IS GONNA GO INTO THE 2D SECTION */
	struct Proof_API SpriteComponent:public Component {
		glm::vec4 Colour = {1.0f,1.0f,1.0f,1.0f};
		TransformComponent SpriteTransfrom;
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		uint32_t StartIndexSlot =0;
	};
}