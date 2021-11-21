#pragma once
#include "Proof/Core/Layer.h"
#include "glm/glm.hpp"
#include "Proof/ProofCore.h"
#include "MainWindow/SceneHierachyPanel.h"
#include "MainWindow/ContentBrowserPanel.h"
#include "Proof/Scene/SceneSerializer.h"
#include "MainWindow/Panel.h"
namespace Proof {
	
	class Proof_API Editore3D : public Layer {
	public:
		Editore3D();
		~Editore3D();
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnImGuiDraw(FrameTime DeltaTime) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void CreateMaterialEdtior(class MaterialAsset* material);
		void CreateMeshEditor(class MeshAsset* mesh);
		
		Count<Texture2D>m_CheckeboardTexture;
	private:
		void Logger();
		bool Docking = false;
		void ViewPort();
		void MainToolBar();
		glm::vec2 m_ViewportBounds[2];
		Count<Texture2D>m_PlayButtonTexture;
		Count<Texture2D>m_PauseButtonTexture;
		Count<Texture2D>m_SimulateButtonTexture;
		Count<Texture2D>m_StopButtonTexture;
		glm::vec2 _ViewPortSize;
		void SetDocking(bool *p_open);
		glm::vec2 _ViewPortLocation;
		bool ViewPortResize = false;
		Count<class World>ActiveWorld =nullptr;
		Count<class World>m_PlayWorld =nullptr;

		int GuizmoType = (1u << 0)| (1u << 1)| (1u << 2);// imgui bit stuff
		class SceneHierachyPanel m_WorldHierachy;
		class ContentBrowserPanel m_CurrentContentBrowserPanel ={this};
		friend class Renderer3D;
		friend class Renderer;

		void NewWorld(bool Save);
		void OpenWorld();
		void Save();
		std::unordered_map<UUID,Panel*>m_AllPanels;
		void PlayWorld();
		void SimulateWorld();
		void SetWorldEdit();
		void PauseWorld();
		void Save(const std::string& Path);

		std::vector<std::string>CubeMapPaths; 
		Count<Shader> m_SkyBoxShader;
		Count<VertexBuffer> m_SkyBoxBuffer;
		Count<VertexArray> m_SkyBoxVertexArray;
		Count<CubeMap> m_CubeMap;
	};
}