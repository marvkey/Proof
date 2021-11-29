#pragma once
#include <unordered_map>
#include <array>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer3DCore.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/RendererAPI.h"
namespace Proof
{
	class PhysicalBasedRenderer;
	enum class RenderTechnique{
		None=0,
		DeferedRendering,
		FowardRendering,
		FowardPlusRendering,
	};
	struct RendererData;
	
	struct PhysicalBasedRendererVertex;
	struct Proof_API DeferedRenderingData {
		Count<class FrameBuffer> Gbuffer;
		Count<class Texture2D>GPosition;
		Count<class Texture2D>GAlbedo;
		Count<class Texture2D>GNormal;
		Count<class Texture2D>GMaterial;
		Count<class RenderBuffer>RenderBuffer;
		Count<class Shader>MeshShader;
		Count<class Shader>LightShader;
		DeferedRenderingData();
		DeferedRenderingData(uint32_t width, uint32_t height);
	};
	struct Proof_API FowardRenderingData {
		Count<class Shader> m_Shader;
		FowardRenderingData();
	};
	struct RendererData{
	public:
		RendererData() =default;
		struct RenderStats {
		public:
			RenderStats()=default;
			uint32_t DrawCalls =0;
			uint32_t Instances =0;
			uint32_t AmountLight =0;
			uint32_t AmountDirectionalLight=0;
			uint32_t AmountPointLight =0;
			uint32_t AmountSpotLight =0;
		};
		struct DeferedRendererData {
		public:
			uint32_t GetPositionTextureID() {
				return m_PositionTexture;
			}
			uint32_t GetNormalTextureID() {
				return m_NormalTexture;
			}
			uint32_t GetAlbedoTexture() {
				return m_AlbedoTexture;
			}
			DeferedRendererData() =default;
		private:
			friend class Renderer3DPBR;
			uint32_t m_PositionTexture = 0;
			uint32_t m_NormalTexture = 0;
			uint32_t m_AlbedoTexture = 0;
		};
		struct FowardRendererData{
			
		};

		struct FowardPlusData{
			
		};
		struct RendererSettings {
			RenderTechnique Technique= RenderTechnique::FowardRendering;
			DrawType Draw= DrawType::Triangles;
		};
		RenderStats Stats;
		DeferedRendererData DeferedData;
		FowardRendererData FowardData;
		RendererSettings RenderSettings;
		friend class Renderer3DPBR;
	private:
		void Reset(){
			Stats = RenderStats();
			DeferedData = DeferedRendererData();
		}
	};
	struct Proof_API PhysicalBasedRenderer { /// Needs to be Renaimed
		Count<class VertexBuffer> m_VertexBuffer;
		Count<class Shader> m_Shader;
		std::unordered_map<uint32_t,uint32_t> m_AmountMeshes;
		std::unordered_map<uint32_t,class MeshComponent> m_Meshes;
		std::unordered_map<uint32_t,uint32_t > m_MeshesEndingPositionIndexTransforms;
		std::vector<PhysicalBasedRendererVertex>m_Transforms;
		bool SceneHasAmountMeshes(uint32_t ID) { return m_AmountMeshes.find(ID) != m_AmountMeshes.end(); };
		static Count<class Texture2D>m_WhiteTexture;
		DeferedRenderingData m_DeferedRendering;
		FowardRenderingData m_FowardRendering;
	};
	
	
	struct Proof_API PhysicalBaseRendererGuide{
	public:
		static const uint32_t s_MaxMesh = 1000;
	};

	struct Proof_API PhysicalBasedRendererVertex{
	
		PhysicalBasedRendererVertex(const glm::mat4& transform, const Material& temp,int usingMaterial):
		m_Transform(transform)
		{
			m_AlbedoColour =temp.m_Colour;
			m_AO= temp.m_AO;
			m_Roughnes =temp.m_Roughness;
			m_Matallness = temp.m_Metallness;
			m_UsingMaterial =usingMaterial;
		}
		glm::mat4 m_Transform;
		Vector<float> m_AlbedoColour;
		float m_Matallness = 0.f;
		float m_Roughnes = 0.f;
		float m_AO = 0.f;
		int m_UsingMaterial =false;
	};
	class Renderer3DPBR {
	public:
		static void Init();
		static void BeginContext(class EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec = RendererData());
		static void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector<>& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec = RendererData());
		static void Draw(class MeshComponent& meshComponent);
		static void Draw(class LightComponent& lightComponent);
		static PhysicalBasedRenderer* GetRenderer();
		static void EndContext();
		static void Reset();
	private:
		static Count<ScreenFrameBuffer> s_RenderFrameBuffer;
		static bool s_InsideContext;
		static void Render();

		/* Defered Rendering */
		static void InitilizeDeferedRendering();
		static void DeferedRender();
		static void DeferedRendererRenderLight();
		static void DeferedRendererRenderMesh();
		static RendererData* s_RendererData;
		/*---------------------------*/

		/* Foward Renderer */
		static void InitilizeFowardRendering();
		static void FowardRenderer();
		/* ------------------------- */


		static void PBRWithFowardRender();
	};
}
