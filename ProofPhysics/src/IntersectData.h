#pragma once
namespace Proof {
	struct IntersectData
	{
	public:
		IntersectData(bool intersects, float distance) :
			m_DoesIntersect(intersects), m_Distance(distance)
		{
		}
		inline bool GetDoesIntersect()const {
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
