#pragma once
namespace ProofPhysicsEngine {
	struct Interval {
		float min; float max;
	};
	struct IntersectData
	{
	public:
		IntersectData(bool intersects, float distance) :
			m_DoesIntersect(intersects), m_Distance(distance)
		{
		}
		inline bool DoesIntersect()const {
			return m_DoesIntersect;
		}
		inline float GetDistance()const {
			return m_Distance;
		}
	private:
		const bool m_DoesIntersect;
		const float m_Distance;
	};
}
