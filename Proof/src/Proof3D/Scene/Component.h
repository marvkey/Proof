#pragma once
#include "Proof3D/Math/Vector.h"
#include "Proof3D/Math/Rotate.h"
#include "ScriptableEntity.h"
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
	Rotate Rotation = {0.0f,0.0f,0.0f};
	Vector Scale = {1.0f,1.0f,1.0f};
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
};

struct Proof_API NativeScriptComponent  {
	ScriptableEntity* Instance = nullptr;
	ScriptableEntity*(*InstantiateScript)();
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
protected:
	virtual void HereForeCodePurpose() {};
};