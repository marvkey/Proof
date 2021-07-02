#pragma once
#include "Proof3D/Math/Vector.h"
#include "Proof3D/Math/Rotate.h"
#include "ScriptableEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Proof{
	struct Proof_API Component{
	public:
		virtual ~Component(){}
	private:
		std::string Name;
	};
	struct Proof_API TagComponent {
		TagComponent() = default;
		void AddTag(const std::string& Tag) {
			Tags.emplace_back(Tag);
		}
		bool HasTag(const std::string& Tag) {
			for (const std::string& TagName : Tags) {
				if (Tag == TagName) {
					return true;
				}
			}
			return false;
		}
		void SetName(const std::string& Name){tag = Name; if(m_EntityOwner != nullptr)m_EntityOwner->Name =Name; }
		std::string GetName(){return tag;}
	private:
		std::string tag ="null";
		std::vector<std::string> Tags;
		friend class Entity;
		friend class World;
		Entity* m_EntityOwner = nullptr;
	};

	struct Proof_API TransformComponent : Component{
		Vector Location = {0.0f,0.0f,0.0f};
		Rotate Rotation = {1.0f,1.0f,1.0f};
		Vector Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		virtual ~TransformComponent(){PF_ENGINE_INFO("DELETE TRANSFORM");};
	};

	struct Proof_API NativeScriptComponent {
		class ScriptableEntity* Instance = nullptr;
		class ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);
		template<class T,typename... Args>
		void Bind(Args... arg) {
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T(arg...)); };
			DestroyScript = [](NativeScriptComponent* NSC) {delete NSC->Instance; NSC->Instance = nullptr; };
		}
	private:
		Entity* EntityOwner = nullptr;
		friend class Entity;
		friend class World;
	};

	struct Proof_API MeshComponent {
		TransformComponent Transform;
		TagComponent Tags;
		MeshComponent() {}
		class Model* GetModel() {
			return m_Mesh;
		}
		class Model* m_Mesh = nullptr;
		uint32_t GetID();
	private:
		Entity* m_EntityOwner;
		friend class Entity;
		friend class World;
		std::string Name = "Static Mesh";
	};
}