#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Renderer2D.h"
namespace Proof
{
	class DebugRenderer : public RefCounted
	{
	public:
		using RenderQueue = std::vector<std::function<void(Count<Renderer2D>)>>;
	public:
		DebugRenderer() = default;
		~DebugRenderer() = default;

		void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color = glm::vec4(1.0f));
		void DrawRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec4& color = glm::vec4(1.0f));
		//lenght is gotten form the direction
		void DrawRay(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color = glm::vec4(1.0f));
		//void DrawQuadBillboard(const glm::vec3& translation, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));

		//void SetLineWidth(float thickness);

		RenderQueue& GetRenderQueue() { return m_RenderQueue; }
		void ClearRenderQueue() { m_RenderQueue.clear(); }
	private:
		RenderQueue m_RenderQueue;
	};
}