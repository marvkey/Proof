#include "Proofprch.h"
#include "WorldRendererPanel.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/ImGui/UIWidgets.h"
namespace Proof
{

	WorldRendererPanel::WorldRendererPanel()
	{
	}

	static void WorldRendererPanelImageMip(const std::string& name, uint32_t& mip,Count<Image2D> image)
	{
		const float size = ImGui::GetContentRegionAvail().x;
		UI::AttributeLabel(name);
		UI::AttributeSlider(fmt::format("{} Mip", name), mip, 0, image->GetSpecification().Mips - 1);
		UI::ImageMip(image, mip, { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });
	}
	static void WorldRendererPanelImageMip(const std::string& name, uint32_t& mip, Count<Texture2D> texture)
	{
		WorldRendererPanelImageMip(name, mip, texture->GetImage());
	}

	static void WorldRendererPanelImage(const std::string& name, Count<Image2D> image)
	{
		const float size = ImGui::GetContentRegionAvail().x;
		UI::AttributeLabel(name);
		UI::Image(image, { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });
	}

	static void WorldRendererPanelTexture(const std::string& name, Count<Texture2D> texture)
	{
		WorldRendererPanelImage(name, texture->GetImage());
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
				//Renderer::GetShaderLibrary()->ForEachShader([&](Count<Shader> shader) 
				//	{
				//
				//		UI::PushID();
				//		if (UI::AttributeButton(shader->GetName(), "Reload"))
				//			shader->Reload();
				//		UI::PopID();
				//
				//	});

				static std::string searchedString;
				UI::Widgets::SearchWidget(searchedString);
				ImGui::Indent();
				auto& shaders = Renderer::GetShaderLibrary()->GetShaderMap();
				for (auto& [name, shader] : shaders)
				{
					if (!UI::IsMatchingSearch(name, searchedString))
						continue;

					ImGui::Columns(2);
					ImGui::Text(name.c_str());
					ImGui::NextColumn();
					std::string buttonName = fmt::format("Reload##{0}", name);
					if (ImGui::Button(buttonName.c_str()))
						shader->Reload();
					ImGui::Columns(1);
				}
				ImGui::Unindent();

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
			if (UI::AttributeTreeNode("HizSSR",false))
			{
				WorldRendererPanelImage("FinalImage", m_WorldRenderer->m_HizSSR.OutputImage);
				static uint32_t Oldhzbmip = 0;
				WorldRendererPanelImageMip("HZB Depth ", Oldhzbmip, m_WorldRenderer->m_SSR.HierarchicalDepthTexture);

				UI::EndTreeNode();
			}
			if (UI::AttributeTreeNode("NewSSR",true))
			{
				if (UI::AttributeTreeNode("Debug Views", false))
				{
					if (m_WorldRenderer->m_ResourcesCreatedGPU)
					{
						WorldRendererPanelImage("FinalImage", m_WorldRenderer->m_NewSSR.ApplyPass->GetOutput(0).As<Image2D>());

						static uint32_t Hzbmip = 0;
						WorldRendererPanelImageMip("HZB Depth", Hzbmip, m_WorldRenderer->m_NewSSR.HierarchalDepthDownSamplerTexture);
						static uint32_t Oldhzbmip = 0;
						WorldRendererPanelImageMip("HZB Depth OldSSR", Oldhzbmip, m_WorldRenderer->m_SSR.HierarchicalDepthTexture);;

						ImGui::Separator();
						//WorldRendererPanelTexture("ScramblingTexture", Renderer::GetSPP1().ScramblingTexture);
						//WorldRendererPanelTexture("SobolTexture", Renderer::GetSPP1().SobolTexture);
						WorldRendererPanelImage("Blue NoiseImage", m_WorldRenderer->m_NewSSR.BlueNoiseImage);

						ImGui::Separator();

						WorldRendererPanelImage("Variance",  m_WorldRenderer->m_NewSSR.Variance);
						WorldRendererPanelImage("Previous Variance", m_WorldRenderer->m_NewSSR.PreviousVariance);

						WorldRendererPanelImage("Radiance",m_WorldRenderer->m_NewSSR.Radiance);
						WorldRendererPanelImage("Previous Radiance", m_WorldRenderer->m_NewSSR.PreviousRadiance);

						WorldRendererPanelImage("ExtractRoughness", m_WorldRenderer->m_NewSSR.ExtractRoughness);
						WorldRendererPanelImage("Previous ExtractRoughness", m_WorldRenderer->m_NewSSR.PreviousExtractRoughness);

						WorldRendererPanelImage("SampleCount", m_WorldRenderer->m_NewSSR.SampleCount);
						WorldRendererPanelImage("Previous SampleCount", m_WorldRenderer->m_NewSSR.PreviousSampleCount);

						WorldRendererPanelImage("Reproject", m_WorldRenderer->m_NewSSR.ReprojectImage);
						WorldRendererPanelImage("Previous ReprojectImage", m_WorldRenderer->m_NewSSR.PreviousReprojectImage);

						WorldRendererPanelImage("Average Radiance", m_WorldRenderer->m_NewSSR.AverageRadiance);

					}
					UI::EndTreeNode();
				}
				UI::EndTreeNode();
			}
			/*
			if (UI::AttributeTreeNode("SSR"))
			{
				auto& ssrOptions = m_WorldRenderer->SSRSettings;

				UI::BeginPropertyGrid();
				UI::AttributeBool("Enable SSR", ssrOptions.Enabled);
				UI::AttributeBool("Enable Cone Tracing", ssrOptions.EnableConeTracing, "Enable rough reflections.");


				UI::AttributeDrag("Brightness", ssrOptions.Brightness, 0.001f, 0.0f, 1.0f);
				UI::AttributeDrag("Depth Tolerance", ssrOptions.DepthTolerance, 0.01f, 0.0f, std::numeric_limits<float>::max());
				UI::AttributeDrag("Roughness Depth Tolerance", ssrOptions.RoughnessDepthTolerance, 0.33f, 0.0f, std::numeric_limits<float>::max(),
					"The higher the roughness the higher the depth tolerance.\nWorks best with cone tracing enabled.\nReduce as much as possible.");
				UI::AttributeDrag("Horizontal Fade In", ssrOptions.FadeIn.x, 0.005f, 0.0f, 10.0f);
				UI::AttributeDrag("Vertical Fade In", ssrOptions.FadeIn.y, 0.005f, 0.0f, 10.0f);
				UI::AttributeDrag("Facing Reflections Fading", ssrOptions.FacingReflectionsFading, 0.01f, 0.0f, 2.0f);
				UI::AttributeDrag("Luminance Factor", ssrOptions.LuminanceFactor, 0.001f, 0.0f, 2.0f, "Can break energy conservation law!");
				UI::AttributeSlider("Maximum Steps", ssrOptions.MaxSteps, 1, 200);
				UI::AttributeBool("Half Resolution", ssrOptions.HalfRes);
				UI::EndPropertyGrid();

				if (UI::AttributeTreeNode("Debug Views", false))
				{
					if (m_WorldRenderer->m_ResourcesCreatedGPU)
					{
						const float size = ImGui::GetContentRegionAvail().x;
						static int32_t Hzbmip = 0;
						UI::AttributeLabel("HZB DepthTexture");
						UI::AttributeSlider("HZB DepthTexture Mip", Hzbmip, 0, (int)m_WorldRenderer->m_SSR.HierarchicalDepthTexture->GetMipLevelCount() - 1);
						UI::ImageMip(m_WorldRenderer->m_SSR.HierarchicalDepthTexture->GetImage(), Hzbmip, { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });

						static int32_t visibilitymip = 0;
						UI::AttributeLabel("Visibility Texture(Pre-integration)");
						UI::AttributeSlider("Visibility Texture(Pre-integration) Mip", visibilitymip, 0, (int)m_WorldRenderer->m_SSR.VisibilityTexture->GetMipLevelCount() - 1);
						UI::ImageMip(m_WorldRenderer->m_SSR.VisibilityTexture->GetImage(), visibilitymip, {size, size * (0.9f / 1.6f)}, {0, 1}, {1, 0});

						
						static int32_t mip = 0;
						UI::AttributeSlider("Pre-convoluted Mip", mip, 0, (int)m_WorldRenderer->m_SSR.PreConvolutedTexture->GetMipLevelCount() - 1);
						UI::ImageMip(m_WorldRenderer->m_SSR.PreConvolutedTexture->GetImage(), mip, { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });

						UI::AttributeLabel("SSRTexture");
						UI::Image(m_WorldRenderer->m_SSR.SSRImage, { size, size * (0.9f / 1.6f) }, { 0, 1 }, { 1, 0 });

					}
					UI::EndTreeNode();
				}
				UI::EndTreeNode();
			}
			*/
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

						UI::AttributeDrag("Intensity", hbao.Intensity, 0.05f, 0.01,15.f);
						UI::AttributeDrag("Radius", hbao.Radius, 0.05f, 0.01f,10.0f);
						UI::AttributeDrag("Bias", hbao.Bias, 0.02f, 0.01f,0.95f);
						UI::AttributeDrag("Blur Sharpness", hbao.BlurSharpness, 0.5f, 0.0f, 100.f);

						UI::EndPropertyGrid();
						if (UI::AttributeTreeNode("Debug Views", false))
						{
							


							{
								static int32_t imageOuput = 0;
								UI::AttributeSlider("imageOutput", imageOuput, 0, 7);
								static int32_t imageLayer = 0;
								UI::AttributeSlider("ImageLayer", imageLayer, 0,
									m_WorldRenderer->m_AmbientOcclusion.HBAO.DeinterleavePass[0]->GetOutput(0).As<Image2D>()->GetSpecification().Layers-1
								);

								auto deinterLeaveImage = m_WorldRenderer->m_AmbientOcclusion.HBAO.DeinterleavePass[0]->GetOutput(imageOuput).As<Image2D>();
								float size = ImGui::GetContentRegionAvail().x;

								
								UI::AttributeLabel("DeinterLeavePass(0)");
								UI::ImageLayer(deinterLeaveImage, imageLayer,{ size, size * (1.0f / deinterLeaveImage->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });
								deinterLeaveImage = m_WorldRenderer->m_AmbientOcclusion.HBAO.DeinterleavePass[1]->GetOutput(imageOuput).As<Image2D>();

								UI::AttributeLabel("DeinterLeavePass(1)");
								UI::ImageLayer(deinterLeaveImage, imageLayer,{ size, size * (1.0f / deinterLeaveImage->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });
							}
							ImGui::Separator();
							{
								auto hbaoOutputImage = m_WorldRenderer->m_AmbientOcclusion.HBAO.HBAOOutputImage;
								float size = ImGui::GetContentRegionAvail().x;
								UI::AttributeLabel("HBAOoutputImage");
								static int32_t hbaoLayer = 0;
								UI::AttributeSlider("hbaoLayer", hbaoLayer, 0, hbaoOutputImage->GetSpecification().Layers - 1);
								UI::ImageLayer(hbaoOutputImage, hbaoLayer,{size, size * (1.0f / hbaoOutputImage->GetAspectRatio())}, {0, 1}, {1, 0});
							}
							ImGui::Separator();

							{
								auto reinterLeaveImage = m_WorldRenderer->m_AmbientOcclusion.HBAO.ReinterleavePass->GetOutput(0);
								float size = ImGui::GetContentRegionAvail().x;
								UI::AttributeLabel("ReinterLeavePass");
								UI::Image(reinterLeaveImage, { size, size * (1.0f / reinterLeaveImage->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });
							}
							ImGui::Separator();

							{
								auto blurImage = m_WorldRenderer->m_AmbientOcclusion.HBAO.BlurPass[0]->GetOutput(0);
								float size = ImGui::GetContentRegionAvail().x;
								UI::AttributeLabel("BlurImage(0)");
								UI::Image(blurImage , { size, size * (1.0f / blurImage->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });
								blurImage = m_WorldRenderer->m_AmbientOcclusion.HBAO.BlurPass[1]->GetOutput(0);

								UI::AttributeLabel("BlurImage(1)");
								UI::Image(blurImage, { size, size * (1.0f / blurImage->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });
							}

							{
								UI::AttributeLabel("Normal Texture");

								auto viewNormalImage = m_WorldRenderer->m_GeometryPass->GetOutput(1);
								float size = ImGui::GetContentRegionAvail().x;
								UI::Image(viewNormalImage, { size, size * (1.0f / viewNormalImage->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });

							}

							{
								UI::AttributeLabel("Depth Texture");

								auto depthTexture = m_WorldRenderer->m_PreDepthPass->GetOutput(0);
								float size = ImGui::GetContentRegionAvail().x;
								UI::Image(depthTexture, { size, size * (1.0f / depthTexture->GetAspectRatio()) }, { 0, 1 }, { 1, 0 });

							}
							ImGui::Separator();

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

