#pragma once
#include "Proof/Core/Core.h"
#include "Camera/EditorCamera.h"
#include "Camera/OrthagraphicCamera.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Component.h"
#define ENTT_ID_TYPE uint64_t
#include "entt/entt.hpp"	
#include "entt/entity/group.hpp"
#include <tuple>
#include <variant>
class FrameTime;
namespace entt {
	using registry64 = basic_registry<uint64_t>; 
};
namespace Proof{
	enum class WorldState 
	{	
		Play,
		Pause,
		Simulate,
		Edit
	};
	
	class Proof_API World {
	public:
		using EntityID = UUID;
		World(const std::string& name = "Default World",UUID ID = UUID()) :
			m_WorldID(ID),Name(name)
		{

		}
		~World() {
			//m_Registry.on_construct<ScriptComponent>();
		}
		
		World(World&)=default;

		bool HasEntity(EntityID ID)const;
		bool HasEntity(EntityID ID);
		void OnUpdateEditor(FrameTime DeltaTime);
		void OnUpdateRuntime(FrameTime DeltaTime);
		void OnSimulatePhysics(FrameTime DeltaTime);
		bool HasWorldCamera();
		class Entity GetWorldCameraEntity();

		class Entity CreateEntity(const std::string& EntName= "Empty Entity");
		class Entity CreateEntity(const std::string& EntName,EntityID ID);
		class Entity CreateEntity(Entity entity,bool includeChildren=true);
		WorldState GetState() {
			return m_CurrentState;
		}
		template<class F>
		void ForEachEntity(F func) {
			for (uint64_t i = 0; i < m_Registry.entities.size(); i++) {
				Entity created{ m_Registry[i],this };
				func(created);
			}
		}
		template<class F>
		void ForEachEntityBackwards(F func) {
			for (auto it = m_Registry.entities.crbegin(); it != m_Registry.entities.crend(); ++it) {
				Entity created{ *it,this };
				func(created);
			}
		}
		template<class ...T, class F>
		void ForEachEnitityWith(F func) {
			const auto& entitiyView = m_Registry.view<T...>();
			for (auto& entity : entitiyView) {
				Entity created{ entity,this };
				func(created);
			}
		}
		
		template <class ...T, class Func>
		void ForEachComponent(Func func) {
			auto view = m_Registry.view<T...>();
			view.pick_and_each(std::move(func), std::index_sequence_for<T...>{});
		}
		// returns number entities with componet
		// if more than one componenet passed it returns number entities with that combiniation of component
		template <class ...T>
		uint32_t GetNumComponents() {
			const auto& group = m_Registry.group<T>();
			return group.size();
		}
		
		static Count<World> Copy(Count<World> other);
		virtual void StartRuntime();
		virtual void EndRuntime();
		const std::string& GetName()const{return Name;};
		const std::string& GetPath()const{return m_Path;}
		void DeleteEntity(class Entity& ent,bool deleteChildren =true);

		//EditorCamera m_EditorCamera ={200,200};
		class PhysicsEngine* GetPhysicsEngine() { return m_PhysicsEngine; };

		Entity GetEntity(UUID id);

		Entity FindEntityByTag(const std::string& tag);

		Vector GetWorldLocation(Entity entity) const;
		Vector GetWorldRotation(Entity entity) const;
		Vector GetWorldScale(Entity entity) const;
		glm::mat4 GetWorldTransform(Entity entity) const;

	private:
		entt::registry64 m_Registry;
		class PhysicsEngine* m_PhysicsEngine =nullptr;
		uint32_t m_LastFrameWidth,m_LastFrameHeight;
		void OnUpdate(FrameTime DeltaTime);
		WorldState m_CurrentState=WorldState::Edit;
		UUID m_WorldID;
		void CreateIBlTexture(const std::string& filePath);
		std::string Name = "DefaultWorld";
		template<class T>
		void OnComponentAdded(EntityID ID, T* component) {};
		
		
		std::string m_Path;
		class OrthagraphicCamera SceneCamera { -1.0f,1.0f,-1.0f,1.0f };
		Count<CubeMap> m_WorldCubeMap;
		Count<CubeMap> m_WorldCubeMapIrradiance;
		Count<CubeMap> PrefelterMap;

		Count<HDRTexture>m_WorldIBLTexture;
		Count<VertexBuffer> m_IBLSkyBoxBuffer;
		Count<VertexArray>m_IBLSkyBoxVertexArray;

		Count<FrameBuffer> m_CaptureFBO;
		Count<Shader>equirectangularToCubemapShader;
		Count<Texture2D> m_brdflTexture;

		Count<Shader> backgroundShader;
		Count<Shader> IrradianceShader;
		Count<Shader>prefilterShader;
		Count<Shader>brdfShader;

		template <class T>
		T* GetComponent(Entity entity);
		friend class SceneHierachyPanel;
		friend class Entity;
		friend class SceneSerializer;
		friend class Editore3D;
		friend class RendererBase;
		friend class WorldRenderer;
		friend class PhysicsEngine;
		
};
}
