#pragma once
#include "Camera/EditorCamera.h"
#include "Camera/OrthagraphicCamera.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Component.h"
#define ENTT_ID_TYPE uint64_t
#include "entt/entt.hpp"	
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
		World();
		World(const std::string& path){
			CreateIBlTexture(path);
		}
		World(World&)=default;
		void OnUpdateEditor(FrameTime DeltaTime,uint32_t width,uint32_t height,bool usePBR = false);
		void OnUpdateEditorNoDraw(FrameTime DeltaTime,uint32_t width,uint32_t height);
		void OnUpdateRuntime(FrameTime DeltaTime,uint32_t width,uint32_t height);
		void OnSimulatePhysics(FrameTime DeltaTime,uint32_t width,uint32_t height);

		class Entity CreateEntity(const std::string& EntName= "Empty Entity");
		class Entity CreateEntity(const std::string& EntName, UUID ID);
		class Entity CreateEntity(Entity entity,bool includeChildren=true);
		
		template<class F>
		void ForEachEntity(F func) {
			for (uint64_t i = 0; i < m_Registry.entities.size(); i++) {
				func(Entity{ m_Registry[i],this });
			}
		}
		template<class F>
		void ForEachEntityBackwards(F func) {
			for (auto it = m_Registry.entities.crbegin(); it != m_Registry.entities.crend(); ++it) {
				func(class Entity{ *it,this });
			}
		}
		template<class T, class F>
		void ForEachEntitiesWithSingle(F func) {
			auto& entitiyView = m_Registry.view<T>;
			for (auto entity : entitiyView) {
				func(Entity{ entity,this });
			}
		}
		
		template<class...T, class F>
		void ForEachEntitiesWithMultiple(F func) {
			auto entitiygroup = m_Registry.group<T...>();
			for (auto& entity : entitiygroup) {
				func(Entity{ entity,this });
			}
		}

		/*
		// WOKR ON THIS
		template<class ...T>
		void ForEachComponentMultiple(void (*func)(std::tuple<T&...> temp) ) {
			auto componentgroup = m_Registry.group<T...>();
			for (auto& entity : componentgroup) {
				func(componentgroup.get(entity));
			}
		}
		// WOKR ON THIS
		template<class ...T,class F>
		void ForEachComponentMultipleV2(F func(std::tuple<T&...> temp)) {
			auto componentgroup = m_Registry.group<T...>();
			for (auto& entity : componentgroup) {
				func(componentgroup.get(entity));
			}
		}
		template<class ...T>
		void ForEachComponentV3(const std::function<void(std::tuple<T...> temp)>& func) {
			auto componentgroup = m_Registry.group<T...>();
			for (auto& entity : componentgroup) {
				func(componentgroup.get(entity));
			}
		}
		*/
		static Count<World> Copy(Count<World> other);
		virtual void EndRuntime();
		virtual void StartRuntime();
		entt::registry64 m_Registry;
		const std::string& GetName()const{return Name;};
		const std::string& GetPath()const{return m_Path;}
		friend class WorldRenderer;
		void DeleteEntity(class Entity& ent,bool deleteChildren =true);

		EditorCamera m_EditorCamera ={200,200};
	private:
		class PhysicsEngine* m_PhysicsEngine =nullptr;
		class CameraComponent* m_ActiveCamera = nullptr;
		uint32_t m_LastFrameWidth,m_LastFrameHeight;
		void OnUpdate(FrameTime DeltaTime,uint32_t m_Width,uint32_t m_Height,bool usePBR =false);
		WorldState m_CurrentState=WorldState::Edit;

		void CreateIBlTexture(const std::string& filePath);
		std::string Name = "DefaultWorld";
		template<class T>
		void OnComponentAdded(UUID ID, T* component) {};
		
		
		std::string m_Path;
		class OrthagraphicCamera SceneCamera { -1.0f,1.0f,-1.0f,1.0f };
		Count<CubeMap> m_WorldCubeMap;
		Count<CubeMap> m_WorldCubeMapIrradiance;
		Count<CubeMap> PrefelterMap;

		Count<HDRTexture>m_WorldIBLTexture;
		Count<VertexBuffer> m_IBLSkyBoxBuffer;
		Count<VertexArray>m_IBLSkyBoxVertexArray;

		Count<FrameBuffer> m_CaptureFBO;
		Count<RenderBuffer> m_CaptureRBO;
		Count<Shader>equirectangularToCubemapShader;
		Count<Texture2D> m_brdflTexture;

		Count<Shader> backgroundShader;
		Count<Shader> IrradianceShader;
		Count<Shader>prefilterShader;
		Count<Shader>brdfShader;

		friend class SceneHierachyPanel;
		friend class Entity;
		friend class SceneSerializer;
		friend class Component;
		friend class Editore3D;
		friend class Renderer;
	};
}
