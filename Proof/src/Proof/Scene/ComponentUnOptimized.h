#pragma once
#include "Component.h"
#include "Entity.h"
#include <iostream>
#include <vector>
#include <algorithm>
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
		void SwapEntityOwner(Entity& neweOwner){
			if (neweOwner.GetID() == GetOwner().GetID()) {
				PF_WARN("cannot add enity as owenr of entity");
				return;
			}
			auto it = std::find(neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.begin(),neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.end(),neweOwner);
			if (it == neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.end()){
				if(HasEntityOwner()==true){
					GetEntityOwner().GetComponent<SubEntityComponet>()->RemoveSubEnity(this->GetOwner());
				}
				m_EntitySubOwner = neweOwner;
				neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.emplace_back(GetOwner());
			}
		}
		void AddSubEntity(Entity& subEntity) {
			if(subEntity.GetID() == GetOwner().GetID()){
				PF_WARN("cannot add enity as owenr of entity");
				return;
			}
			subEntity.GetComponent<SubEntityComponet>()->m_EntitySubOwner = this->GetOwner();
			auto it = std::find(m_AllSubEntity.begin(),m_AllSubEntity.end(),subEntity);
			if (it == m_AllSubEntity.end())
				m_AllSubEntity.emplace_back(subEntity);
		}
	private:
		
		void RemoveSubEnity(const Entity& ent) {
			if (m_AllSubEntity.size() == 0)
				return;

			auto it = std::find(m_AllSubEntity.begin(), m_AllSubEntity.end(), ent);
			if(it != m_AllSubEntity.end()){
				m_AllSubEntity.erase(it);
			}
		}

		std::vector<Entity>m_AllSubEntity;
		Entity m_EntitySubOwner ={};
		friend class Entity;
		friend class SceneHierachyPanel;
		friend class World;
		friend class SceneSerializer;
	};
}