#include "Proofprch.h"
#include "DebugRenderer.h"

namespace Proof
{
	void DebugRenderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		m_RenderQueue.emplace_back([p0, p1, color](Count<Renderer2D> renderer)
			{
				renderer->DrawLine(p0, p1, color);
			});
	}
	void DebugRenderer::DrawRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec4& color)
	{
		m_RenderQueue.emplace_back([origin, direction, length,color](Count<Renderer2D> renderer)
			{
				renderer->DrawRay(origin, direction, length, color);
			});
	}
	void DebugRenderer::DrawRay(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color)
	{
		m_RenderQueue.emplace_back([origin, direction, color](Count<Renderer2D> renderer)
			{
				renderer->DrawRay(origin, direction, color);
			});
	}
}
