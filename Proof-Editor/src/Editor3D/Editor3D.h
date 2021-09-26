#pragma once
#include "Proof/Core/Layer.h"
#include "glm/glm.hpp"
#include "Proof/ProofCore.h"
#include "MainWindow/SceneHierachyPanel.h"
#include "MainWindow/ContentBrowserPanel.h"
#include "Proof/Scene/SceneSerializer.h"
namespace Proof {
	class Proof_API Editore3D : public Layer {
	public:
		Editore3D();
		~Editore3D();
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnImGuiDraw() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void SetMaterialEditor(class MaterialAsset& material);

	private:
		void Logger();
		bool Docking = false;
		void ViewPort();
		glm::vec2 _ViewPortSize;
		void SetDocking(bool *p_open);
		glm::vec2 _ViewPortLocation;
		bool ViewPortResize = false;
		class World* ActiveWorld =nullptr;

		int GuizmoType =0;
		class SceneHierachyPanel m_WorldHierachy;
		class ContentBrowserPanel m_CurrentContentBrowserPanel ={this};
		friend class Renderer3D;
		friend class Renderer;

		void NewWorld(bool Save);
		void OpenWorld();
		void Save();
		void Editore3D::Save(const std::string& Path);

		std::vector<std::string>CubeMapPaths; 
		Count<Shader> m_SkyBoxShader;
		Count<VertexBuffer> m_SkyBoxBuffer;
		Count<VertexArray> m_SkyBoxVertexArray;
		Count<CubeMap> m_CubeMap;

		void MaterialEditor();

		MaterialAsset* mat;
		uint32_t TempID;
	};
}