#pragma once
#include "Proof3D/Math/Vector.h"
#include "Proof3D/Math/Rotate.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Proof{
	struct Proof_API Component{
	public:
		virtual ~Component(){}
		std::string GetName(){return Name;}
		class Entity* GetOwner(){return m_EntityOwner;};
	protected:
		std::string Name;
		class Entity* m_EntityOwner = nullptr;
	private:
		friend class Entity;
		friend class World;
	};
	struct Proof_API TagComponent :Component {
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
		void SetName(const std::string& name){
			Name = name;
		}
	private:
		std::vector<std::string> Tags;
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
	};

	struct Proof_API TransformComponent : Component{
		Vector Location = {0.0f,0.0f,0.0f};
		Vector Rotation = {0.0f,0.0f,0.0f};
		Vector Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
	};

	struct Proof_API NativeScriptComponent : Component {
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
	};

	struct Proof_API MeshComponent : Component {
		MeshComponent() {}
		class Model* GetModel() {
			return m_Mesh;
		}
		class Model* m_Mesh = nullptr;
		uint32_t GetID();
	private:
		friend class Entity;
		friend class World;
	};
}