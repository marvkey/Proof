#pragma once
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
namespace Proof
{
	class OpenGLRenderer3DPBR {
		static void Init();
		static void BeginContext(class EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec);
		static void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec);
		static void Draw(class MeshComponent& meshComponent, const glm::mat4& positionMatrix);
		static void DrawDebugMesh(class Mesh* mesh, const glm::mat4& transform);
		static void SubmitDirectionalLight(const DirectionalLightComponent& comp, class TransformComponent& transform);
		static void SubmitPointLight(class PointLightComponent& comp, class TransformComponent& transform);
		static void SubmitSpotLight(class SpotLightComponent& comp, class TransformComponent& transform);
		static PhysicalBasedRenderer* GetRenderer();
		static void EndContext();
		static void Reset();
	private:
		friend class Renderer3DPBR;
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