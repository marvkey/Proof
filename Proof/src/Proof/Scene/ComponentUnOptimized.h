#pragma once
#include "Component.h"
#include "Entity.h"
namespace Proof{
	class Proof_API SubEntityComponet:public Component {
	public:
		SubEntityComponet(const SubEntityComponet&) = default;
		SubEntityComponet() = default;
		bool HasEntityOwner()const {
			return (bool)m_EntitySubOwner;

		}
		Entity GetEntityOwner()const {
			return m_EntitySubOwner;
		}
		const std::vector<Entity>& GetAllSubEntity()const {
			return m_AllSubEntity;
		}
	private:
	
		void AddSubEntity(const Entity& ent) {
			m_AllSubEntity.push_back(Entity(ent));
		}
		void RemoveSubEnity(const Entity& ent) {
			auto it = std::find(m_AllSubEntity.begin(),m_AllSubEntity.end(),ent.GetID());
			if (it != m_AllSubEntity.end()) {
				m_AllSubEntity.erase(it);
			}
		}

		std::vector<Entity>m_AllSubEntity;
		Entity m_EntitySubOwner ={};
		friend class Entity;
		friend class SceneHierachyPanel;
		friend class World;
	};
}