#include "Editor3D.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuizmo.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Renderer/Renderer.h"

#include "Proof/Math/MathResource.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/Asset/MaterialAsset.h"
namespace Proof {
	Editore3D::Editore3D() :
		Layer("Editor3D Layer")
	{
	}
	Editore3D::~Editore3D() {
		if(ActiveWorld != nullptr)
			delete ActiveWorld;
	}
	void Editore3D::OnAttach() {
		ActiveWorld =new World();
		m_WorldHierachy.SetContext(ActiveWorld);
		m_WorldHierachy.SetBrowserPanel(&m_CurrentContentBrowserPanel);
		CubeMapPaths.emplace_back("Assets/Textures/skybox/right.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/left.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/bottom.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/top.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/front.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/back.jpg");


   float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


	
	m_SkyBoxShader = Shader::Create("SkyBox Shader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/CubeMapShader.shader");
	m_SkyBoxBuffer = VertexBuffer::Create(&skyboxVertices,sizeof(skyboxVertices));
	m_SkyBoxVertexArray = VertexArray::Create();
	m_SkyBoxVertexArray->AddData(0,3,3*sizeof(float),(void*)0);

	m_SkyBoxShader->UseShader();
	m_SkyBoxShader->SetInt("skybox",0);
	

	m_CubeMap = CubeMap::Create(CubeMapPaths);
	}
	void Editore3D::OnDetach() {
		if(ActiveWorld != nullptr){
			SceneSerializer scerelizer(ActiveWorld);
			scerelizer.SerilizeText(ActiveWorld->GetPath());
		}
	}
	
	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		Layer::OnUpdate(DeltaTime);
		Application::ViewPortWidth = _ViewPortSize.x;
		Application::ViewPortHeight = _ViewPortSize.y;
		if (Input::IsKeyClicked(KeyBoardKey::L)) {
			GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (Input::IsKeyClicked(KeyBoardKey::R)) {
			GuizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		if (Input::IsKeyClicked(KeyBoardKey::S)) {
			GuizmoType = ImGuizmo::OPERATION::SCALE;
		}
		ActiveWorld->OnUpdateEditor(DeltaTime);

		glm::mat4 view = -glm::mat4(glm::mat3(ActiveWorld->EditorCamera.GetCameraView())); /// makes makes the sky box move around player, makes it seem the sky box is very large

		
		glDepthFunc(GL_LEQUAL);
		m_SkyBoxShader->UseShader();
		m_SkyBoxShader->SetMat4("view",view);
		m_SkyBoxShader->SetMat4("projection",ActiveWorld->Projection);

		m_SkyBoxVertexArray->Bind();
		glActiveTexture(GL_TEXTURE0);
		m_CubeMap->Bind();
		glDrawArrays(GL_TRIANGLES,0,36);
		m_SkyBoxVertexArray->UnBind();
		glDepthFunc(GL_LESS);
		
	}

	void Editore3D::OnImGuiDraw() {
		Layer::OnImGuiDraw();
		static bool EnableDocking = true;
		ImGui::ShowDemoWindow();

		SetDocking(&EnableDocking);
		ViewPort();
		m_WorldHierachy.ImGuiRender();
		m_CurrentContentBrowserPanel.ImGuiRender();
		if(ImGui::Begin("Renderer Stastitics")){
			ImGui::TextColored({1.0,0,0,1},"RENDERER SPECS");
			ImGui::Text("Renderer Company: %s",Renderer::GetRenderCompany().c_str());
			ImGui::Text("Graphics Card: %s",Renderer::GetGraphicsCard().c_str());
			ImGui::Text("Graphics Card Verison: %s",Renderer::GetGraphicsCardVersion().c_str());
			ImGui::Text("%.3f ms/frame %.1f FPS",FrameTime::GetFrameMS(),FrameTime::GetFrameFPS());


			ImGui::TextColored({1.0,0,0,1},"RENDERER 3D");
			ImGui::Text("DrawCalls %i",Renderer3D::Render3DStats::DrawCalls);
			ImGui::Text("Number Of MeshInstances %i",Renderer3D::Render3DStats::NumberOfInstances);
			ImGui::Text("Amount Of Meshes Drawn %i",Renderer3D::Render3DStats::AmountDrawn);

			ImGui::TextColored({1.0,0,0,1},"RENDERER 2D");
			ImGui::Text("DrawCalls %i",Renderer2D::Renderer2DStats::m_DrawCalls);
			ImGui::Text("Number of Quads %i",Renderer2D::Renderer2DStats::m_QuadCount);
			ImGui::End();
		}
		MaterialEditor();
	}
	void Editore3D::ViewPort() {
	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		static bool Open = true;
		ImGui::Begin("ViewPort",&Open,ImGuiWindowFlags_NoMove);
		
		ImVec2 ViewPortPanelSize = ImGui::GetContentRegionAvail();
		if (_ViewPortSize != *((glm::vec2*)&ViewPortPanelSize)) {
			_ViewPortSize = {ViewPortPanelSize.x,ViewPortPanelSize.y};
			Window_ViewPortResize::Instance->EventHandled = true;
			Window_ViewPortResize::Instance->X = ViewPortPanelSize.x;
			Window_ViewPortResize::Instance->Y = ViewPortPanelSize.y;
		}
		
		if (ImGui::IsWindowFocused()) {
			Input::ViewPoartHovered = true;
		}
		else {
			Input::ViewPoartHovered = false;
		}
		
		uint32_t Text = Application::GetScreenBuffer()->GetTexture();
		ImGui::Image((ImTextureID)Text,ImVec2{_ViewPortSize.x,_ViewPortSize.y},ImVec2{0,1},ImVec2{1,0});
		// GUIZMOS

		Entity selectedEntity = this->m_WorldHierachy.GetSelectedEntity();

		if(selectedEntity.GetID() !=0){
			/*
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth  =(float) ImGui::GetWindowWidth();
			float windowHeight = (float) ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,windowWidth,windowHeight);

			const glm::mat4& cameraProjection = ActiveWorld->Projection;
			glm::mat4 cameraView = glm::inverse(ActiveWorld->EditorCamera.GetTransform());


			auto& tc = *selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView),glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION(GuizmoType),ImGuizmo::LOCAL,glm::value_ptr(transform));
			
			if(ImGuizmo::IsUsing()&& GuizmoType != -1){
				glm::vec3  translation,rotation,scale;
				MathResource::DecomposeTransform(transform,translation,rotation,scale);

				glm::vec3 deltaRotaiton = Vector(rotation) -tc.Rotation; // prevent gimble

				tc.Location = translation;
				tc.Rotation+=deltaRotaiton;
				tc.Scale =scale;
			}
			*/
		}
		
		/* putting this underneath image because a window only accpet drop target to when item is bound so and image has been bound */
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("World")) {
				std::string Data = (const char*)payload->Data;
				SceneSerializer scerilize(ActiveWorld);
				scerilize.SerilizeText(ActiveWorld->GetPath());
				m_WorldHierachy.m_SelectedEntity ={};
				delete ActiveWorld;
				ActiveWorld = new World();
				SceneSerializer ScerilizerNewWorld(ActiveWorld);
				ScerilizerNewWorld.DeSerilizeText(Data);

				m_WorldHierachy.SetContext(ActiveWorld);
			}
			ImGui::EndDragDropTarget();
		}
		/*----------------------------------------------------------------------------------------------------------------------------*/
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Editore3D::SetDocking(bool * p_open) {
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen){
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0.0f,0.0f));
		ImGui::Begin("Proof Engine",p_open,window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 250.0f; // sets the minimum width of everything

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id,ImVec2(0.0f,0.0f),dockspace_flags);
		}
		
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New...")) {
					NewWorld(true);
				}
				if (ImGui::MenuItem("Save")) {
					if (ActiveWorld->m_Path.empty() != true) {
						Save();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	void Editore3D::NewWorld(bool Save) {
		if(Save ==true && ActiveWorld != nullptr){
			SceneSerializer scerelizer(ActiveWorld);
			scerelizer.SerilizeText(ActiveWorld->GetPath());
			delete ActiveWorld;
			ActiveWorld = new World();
			m_WorldHierachy.SetContext(ActiveWorld);
		}else{
			if(ActiveWorld != nullptr){
				delete ActiveWorld;
			}
			ActiveWorld = new World();
			m_WorldHierachy.SetContext(ActiveWorld);
			return;
		}
		ActiveWorld = new World();
		m_WorldHierachy.SetContext(ActiveWorld);
	}

	void Editore3D::OpenWorld() {
		std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.ProofAsset)\0 *.ProofAsset\0 ");
		if(FIle.empty() ==false){
			ActiveWorld = new World();
			SceneSerializer scerelizer(ActiveWorld);
			if (scerelizer.DeSerilizeText(FIle) == true) {
				PF_ENGINE_INFO("World Deserilize perfectly");
				m_WorldHierachy.SetContext(ActiveWorld);
				return;
			}else{
				ActiveWorld = new World();
				m_WorldHierachy.SetContext(ActiveWorld);
				PF_ENGINE_ERROR("Deceerilize was created default world");
				return;
			}
		}
		PF_ENGINE_TRACE("No File Selected");
		if(ActiveWorld == nullptr){
			PF_ENGINE_INFO("New World Created cause level was originally nullptr");
			ActiveWorld = new World();
			m_WorldHierachy.SetContext(ActiveWorld);
			return;
		}
		PF_ENGINE_INFO("Back to Default WOrld");
	}

	void Editore3D::Save() {
		if(ActiveWorld != nullptr){
			SceneSerializer scerelizer(ActiveWorld);
			scerelizer.SerilizeText(ActiveWorld->GetPath());

			PF_ENGINE_TRACE("World Saved");
			return;
		}
		PF_ENGINE_ERROR("World is NULL");
	}
	void Editore3D::Save(const std::string& Path){
		if (ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld);
			scerelizer.SerilizeText(Path);

			PF_ENGINE_TRACE("World Saved");
			return;
		}
		PF_ENGINE_ERROR("World is NULL");
	}


	void Editore3D::SetMaterialEditor(MaterialAsset& material) {
		mat = &material;
		TempID = material.GetID();
		PF_ENGINE_INFO("material ID %i",TempID);
	}

	void Editore3D::MaterialEditor() {
		static bool On =true;
		if(ImGui::Begin("Material Editor",&On)){
			if(TempID ==0){
				ImGui::End();
				return;
			}
			if (AssetManager::HasID(TempID)==false ){
				mat =nullptr;
				TempID =0;
				ImGui::End();
				return;
			}

			ImGui::ColorEdit3("Ambient",glm::value_ptr(mat->m_Material.m_Ambient));
			ImGui::ColorEdit3("Diffuse",glm::value_ptr(mat->m_Material.m_Diuffuse));
			ImGui::ColorEdit3("Specular",glm::value_ptr(mat->m_Material.m_Specular));
			ImGui::DragFloat("Metallines",&mat->m_Material.m_Metallness);
			mat->SaveAsset();
			ImGui::End();
		}
	}
}
