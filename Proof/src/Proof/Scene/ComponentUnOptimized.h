#pragma once
#include "Component.h"
#include "Entity.h"
#include <iostream>
#include <vector>
namespace Proof{
	class Proof_API SubEntityComponet:public Component {
	public:
		SubEntityComponet(const SubEntityComponet&) = default;
		SubEntityComponet() = default;
		bool HasEntityOwner()const {
			return (bool)m_EntitySubOwner.GetID()!=0&&m_EntitySubOwner.GetCurrentWorld() !=nullptr;

		}
		Entity GetEntityOwner()const {
			return m_EntitySubOwner;
		}
		const std::vector<Entity>& GetAllSubEntity()const {
			return m_AllSubEntity;
		}
	private:
	
		void AddSubEntity(const Entity& ent) {
			m_AllSubEntity.push_back(ent);
		}
		void RemoveSubEnity(const Entity& ent) {
			if (m_AllSubEntity.size() <= 0)
				return;
			m_AllSubEntity.erase(m_AllSubEntity.begin());
		}

		std::vector<Entity>m_AllSubEntity;
		Entity m_EntitySubOwner ={};
		friend class Entity;
		friend class SceneHierachyPanel;
		friend class World;
		friend class SceneSerializer;
	};
}