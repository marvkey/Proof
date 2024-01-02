#include "Proofprch.h"
#include "WorldRendererPanel.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RenderPass.h"
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
					UI::AttributeBool("DebugPass", m_WorldRenderer->ShadowSetting.RenderDebugPass);
					if (m_WorldRenderer->ShadowSetting.RenderDebugPass)
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
					UI::Image(m_WorldRenderer->m_BloomComputeTextures[bloomDebuggerImage]->GetImageMip(bloomDebuggerMip), { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });


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

			if (UI::AttributeTreeNode("Ambient Occlusion"))
			{
				UI::BeginPropertyGrid();
				UI::AttributeBool("Enabled", m_WorldRenderer->AmbientOcclusionSettings.Enabled);

				UI::PushItemDisabled(!m_WorldRenderer->AmbientOcclusionSettings.Enabled);
				UI::EnumCombo("OcclusionType", m_WorldRenderer->AmbientOcclusionSettings.Type, {},
					{"Horizon-Based Ambient Occlusion"});

				UI::AttributeDrag("ShadowTolerance", m_WorldRenderer->AmbientOcclusionSettings.ShadowTolerance, 0.001f, 0.0f, 1.0f);
				UI::EndPropertyGrid();

				switch (m_WorldRenderer->AmbientOcclusionSettings.Type)
				{
					case AmbientOcclusion::AmbientOcclusionType::HBAO:
					{
						auto& hbao = m_WorldRenderer->AmbientOcclusionSettings.HBAO;
						UI::BeginPropertyGrid();

						UI::AttributeDrag("Intensity", hbao.Intensity, 0.05f, 0.1f, 6.0f);
						UI::AttributeDrag("Radius", hbao.Radius, 0.05f, 0.0f, 8.0f);
						UI::AttributeDrag("Bias", hbao.Bias, 0.02f, 0.0f, 0.95f);
						UI::AttributeDrag("Blur Sharpness", hbao.BlurSharpness, 0.5f, 0.0f, 100.f);

						UI::EndPropertyGrid();
						if (UI::AttributeTreeNode("Debug Views", false))
						{
							auto image = m_WorldRenderer->m_AmbientOcclusion.HBAO.HBAOOutputImage;
							if (image)
							{
								float size = ImGui::GetContentRegionAvail().x;
								static int32_t layer = 0;
								UI::AttributeSlider("Layer", layer, 0, image->GetSpecification().Layers-1);
								UI::ImageLayer(image, layer, { size, size * (1.0f / image->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });
							}

							{

								UI::AttributeLabel("AO-Composite");
								float size = ImGui::GetContentRegionAvail().x;
								UI::Image(m_WorldRenderer->m_AmbientOcclusionCompositePass->GetOutput(0), {size, size * (1.0f / m_WorldRenderer->m_AmbientOcclusionCompositePass->GetOutput(0)->GetAspectRatio())}, {0, 1}, {1, 0});
							}
							UI::EndTreeNode();
						}
						break;
					}
				}
				UI::PopItemDisabled();

				UI::EndTreeNode();
			}
			ImGui::End();

		}
	}
}

