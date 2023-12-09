#include "WorldRendererPanel.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Core/FrameTime.h"
namespace Proof
{

	WorldRendererPanel::WorldRendererPanel()
	{
	}
	void WorldRendererPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen)
	{
		if (ImGui::Begin(dsiplayName, &isOpen))
		{
			if (m_WorldRenderer == nullptr)
			{
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "No Active Renderer");
				ImGui::End();
				return;
			}

			ImGui::Text("Viewport Size: %d, %d", (int)m_WorldRenderer->m_UBScreenData.FullResolution.x, (int)m_WorldRenderer->m_UBScreenData.FullResolution.y);
			ImGui::Text("%f m/s", FrameTime::GetFrameMS());
			ImGui::Text("%f FPS", FrameTime::GetFrameFPS());

			if (UI::AttributeTreeNode("Renderer Settings"))
			{
				UI::BeginPropertyGrid();
				WorldRendererOptions& options = m_WorldRenderer->GeneralOptions;
				UI::EnumCombo("Collider View", options.ShowPhysicsColliders);
				UI::AttributeBool("View LightGrid", options.ShowLightGrid);

				UI::EndPropertyGrid();

				UI::EndTreeNode();
			}


			if (UI::AttributeTreeNode("Shaders", false))
			{
				if (UI::AttributeButton("", "ReloadAll"))
				{
					Renderer::GetShaderLibrary()->ForEachShader([&](Count<Shader> shader) {

						UI::PushID();
							shader->Reload();
						UI::PopID();

					});
				}
				Renderer::GetShaderLibrary()->ForEachShader([&](Count<Shader> shader) 
					{

						UI::PushID();
						if (UI::AttributeButton(shader->GetName(), "Reload"))
							shader->Reload();
						UI::PopID();

					});
				UI::EndTreeNode();
			}

			if (UI::AttributeTreeNode("Shadows"))
			{
				ShadowSetting& shadowSetting = m_WorldRenderer->ShadowSetting;

				UI::BeginPropertyGrid();
				UI::AttributeBool("Soft Shadows", shadowSetting.SoftShadows);
				UI::AttributeDrag("Max Shadow Distance", shadowSetting.MaxShadowDistance);
				UI::AttributeDrag("Shadow Fade", shadowSetting.ShadowFade, 5.0f);
				UI::EndPropertyGrid();

				if (UI::AttributeTreeNode("Cascade Settings"))
				{
					UI::BeginPropertyGrid();

					UI::AttributeBool("CascadeFading", shadowSetting.CascadeFading);
					if (shadowSetting.CascadeFading)
						UI::AttributeDrag("CascadeTransitionFade", shadowSetting.CascadeTransitionFade, 0.05);

					UI::AttributeDrag("CascadeSplitLambda", shadowSetting.CascadeSplitLambda, 0.01);
					UI::AttributeDrag("CascadeNearPlaneOffset", shadowSetting.CascadeNearPlaneOffset, 0.1, Math::GetMinType<float>(), 0, "%.3f", ImGuiSliderFlags_AlwaysClamp);
					UI::AttributeDrag("CascadeFarPlaneOffset", shadowSetting.CascadeFarPlaneOffset, 0.1, 0, Math::GetMaxType<float>(), "%.3f", ImGuiSliderFlags_AlwaysClamp);
					UI::AttributeDrag("ScaleShadowCascadesToOrigin", shadowSetting.ScaleShadowCascadesToOrigin, 0.1, 0, Math::GetMaxType<float>(), "%.3f", ImGuiSliderFlags_AlwaysClamp);

					UI::AttributeBool("UseManualCascadeSplits", shadowSetting.UseManualCascadeSplits);

					if (shadowSetting.UseManualCascadeSplits)
					{
						UI::AttributeDrag("Cascade 0", shadowSetting.CascadeSplits[0], 0.025);
						UI::AttributeDrag("Cascade 1", shadowSetting.CascadeSplits[1], 0.025);
						UI::AttributeDrag("Cascade 2", shadowSetting.CascadeSplits[2], 0.025);
						UI::AttributeDrag("Cascade 3", shadowSetting.CascadeSplits[3], 0.025);
					}
					UI::AttributeBool("DebugPass", shadowSetting.RenderDebugPass);
					if (shadowSetting.RenderDebugPass)
					{

						UI::Image(m_WorldRenderer->GetShadowPassDebugImage(),
							{ ImGui::GetWindowWidth(),ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
						UI::AttributeSlider("Cascade Index", m_WorldRenderer->ShadowSetting.DebugCascade, 0, 3);
					}
					UI::EndPropertyGrid();

					UI::EndTreeNode();
				}

				UI::EndTreeNode();
			}

			ImGui::Separator();

			ImGui::Text("Post Process");


			if (UI::AttributeTreeNode("Bloom"))
			{
				UI::BeginPropertyGrid();
				BloomSettings& bloomSettings = m_WorldRenderer->BloomSettings;
				UI::AttributeBool("Enabled", bloomSettings.Enabled);
				UI::AttributeDrag("Intensity", bloomSettings.Intensity,0.01);
				UI::AttributeDrag("Threshold", bloomSettings.Threshold, 0.01);
				UI::AttributeDrag("UpsampleScale", bloomSettings.UpsampleScale, 0.01);
				UI::AttributeDrag("Knee", bloomSettings.Knee, 0.01);
				UI::EndPropertyGrid();

				if (UI::AttributeTreeNode("Debug Views", false))
				{

					const float size = ImGui::GetContentRegionAvail().x;

					static uint32_t bloomDebuggerImage = 0;
					static uint32_t bloomDebuggerMip = 0;
					UI::AttributeSlider("BloomImage", bloomDebuggerImage, 0, m_WorldRenderer->m_BloomComputeTextures.size() - 1);
					UI::AttributeSlider("BloomMip", bloomDebuggerMip, 0, m_WorldRenderer->m_BloomComputeTextures[bloomDebuggerImage]->GetMipLevelCount() - 1);
					UI::Image(m_WorldRenderer->m_BloomComputeTextures[bloomDebuggerImage]->GetImageMip(bloomDebuggerMip).As<Image>(), { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });


					UI::EndTreeNode();

				}

				UI::EndTreeNode();
			}

			if (UI::AttributeTreeNode("Depth of Field",false))
			{
				UI::BeginPropertyGrid();
				UI::AttributeBool("Enabled", m_WorldRenderer->DOFSettings.Enabled);
				UI::AttributeDrag("FocusDistance", m_WorldRenderer->DOFSettings.FocusDistance, 1, 0);
				UI::AttributeDrag("BlurSize", m_WorldRenderer->DOFSettings.BlurSize, 1, 0);
				UI::EndPropertyGrid();

				if (UI::AttributeTreeNode("Debug Views", false))
				{
					const float size = ImGui::GetContentRegionAvail().x;
					UI::Image(m_WorldRenderer->m_DOFTexture, { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });
					UI::EndTreeNode();
				}
				UI::EndTreeNode();
			}
			ImGui::End();

		}
	}
}

