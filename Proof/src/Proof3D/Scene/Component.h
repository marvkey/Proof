#pragma once
#include "Proof3D/Math/Vector.h"
#include "Proof3D/Math/Rotate.h"
#include "ScriptableEntity.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	glm::mat4 GetTransform() { return Transform; }
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
private:
	glm::mat4 Transform = glm::translate(glm::mat4(1.0f),{Location}) * glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.X),{Rotation})
		* //glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.Y),{Rotation}) * glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.Z),{Rotation}) *
		glm::scale(glm::mat4(1.0f),{Scale});
};

struct Proof_API NativeScriptComponent  {
	class ScriptableEntity* Instance = nullptr;
	class ScriptableEntity*(*InstantiateScript)();
	void (*DestroyScript)(NativeScriptComponent*);
	template<class T, typename... Args>
	void Bind(Args... arg){
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T(arg...)); };
			DestroyScript = [](NativeScriptComponent* NSC) {delete NSC->Instance; NSC->Instance = nullptr; };
	}
private:
	Entity* EntityOwner =nullptr;
	friend class Entity;
	friend class World;
};

struct Proof_API StaticMeshComponent {
	TransformComponent Transform;
	TagComponent Tags;
	Proof::Model* GetModel() {
		return m_Mesh;
	}
	Proof::Model* m_Mesh = nullptr;
private:
	Entity* m_EntityOwner;
	friend class Entity;
	friend class World;
	//friend class Proof::Renderer3D;
	std::string Name="Static Mesh";
};