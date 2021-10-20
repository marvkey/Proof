#pragma once
#include "EntitiyComponentSystem/ECS.h"
#include "Camera/EditorCamera.h"
#include "Camera/OrthagraphicCamera.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/FrameBuffer.h"
class FrameTime;

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
		void OnUpdateEditor(FrameTime DeltaTime,uint32_t width,uint32_t height);
		void OnUpdateRuntime(FrameTime DeltaTime,uint32_t width,uint32_t height);
		void OnSimulatePhysics(FrameTime DeltaTime,uint32_t width,uint32_t height);

		class Entity CreateEntity(const std::string& EntName);
		class Entity CreateEntity(const std::string& EntName,uint32_t ID);

		Count<World> Copy(Count<World> world);
		virtual void EndRuntime();
		std::string GetName(){return Name;};
		class ECS Registry;
		std::string GetPath(){
			return m_Path;
		}
		friend class WorldRenderer;
	private:
		uint32_t m_LastFrameWidth,m_LastFrameHeight;
		void OnUpdate(FrameTime DeltaTime,uint32_t m_Width,uint32_t m_Height);
		WorldState m_CurrentState=WorldState::Edit;

		void CreateIBlTexture(const std::string& filePath);
		std::string Name = "DefaultWorld";
		template<class T>
		void OnComponentAdded(Entity Entity,T* component);
		std::string m_Path;
		EditorCamera m_EditorCamera ={200,200};
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
		friend class Editor3D;
		friend class SceneSerializer;
		friend class Component;
		friend class Editore3D;
		friend class Renderer;
	};
}
