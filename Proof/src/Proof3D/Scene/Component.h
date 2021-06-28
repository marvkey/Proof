#pragma once
#include "Proof3D/Math/Vector.h"
#include "Proof3D/Math/Rotate.h"
#include "ScriptableEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Proof{
	struct Proof_API TagComponent {
		std::string Tag;
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
	private:
		std::vector<std::string> Tags;
	};

	struct Proof_API TransformComponent {
		Vector Location = {0.0f,0.0f,0.0f};
		Rotate Rotation = {1.0f,1.0f,1.0f};
		Vector Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
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
		MeshComponent() { m_ID = 1; }
		class Model* GetModel() {
			return m_Mesh;
		}
		class Model* m_Mesh = nullptr;
		uint32_t GetID() { return m_ID; }
	private:
		Entity* m_EntityOwner;
		friend class Entity;
		friend class World;
		std::string Name = "Static Mesh";
		uint32_t m_ID =1;
	};
}