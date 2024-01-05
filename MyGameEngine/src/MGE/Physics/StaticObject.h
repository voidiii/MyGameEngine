#pragma once
#include "MathAlias.h"
#include "PhysicsObject_Spring.h"

namespace MGE {

	class StaticObject_Line
	{
	public:
		StaticObject_Line(Vec2_Physics LeftStartPoint, Vec2_Physics RightEndPoint, bool BoxBound)
			: m_LeftStartPoint(LeftStartPoint), m_RightEndPoint(RightEndPoint), m_BoxBound(BoxBound)
		{
			m_Normal = Vec2_Physics{ -(m_RightEndPoint.y - m_LeftStartPoint.y), m_RightEndPoint.x - m_LeftStartPoint.x };
			m_Normal = mathter::Normalize(m_Normal);

			m_MidPoint = (m_LeftStartPoint + m_RightEndPoint) / 2.0f;
			//m_UpperBound = max(m_LeftStartPoint.y, m_RightEndPoint.y);
			//m_LowerBound = min(m_LeftStartPoint.y, m_RightEndPoint.y);
			//m_LeftBound = min(m_LeftStartPoint.x, m_RightEndPoint.x);
			//m_RightBound = max(m_LeftStartPoint.x, m_RightEndPoint.x);
		}
		~StaticObject_Line() {}

		inline Vec2_Physics GetLeftStartPoint() const { return m_LeftStartPoint; }
		inline Vec2_Physics GetRightEndPoint() const { return m_RightEndPoint; }
		inline Vec2_Physics GetMidPoint() const { return m_MidPoint; }
		inline Vec2_Physics GetNormal() const { return m_Normal; }
		inline float GetUpperBound() const { return m_UpperBound; }
		inline float GetLowerBound() const { return m_LowerBound; }
		inline float GetLeftBound() const { return m_LeftBound; }
		inline float GetRightBound() const { return m_RightBound; }

		void ApplySupport(Timestep ts, Ref<MassPoint> mass_point);
		bool IsBoxBound() const { return m_BoxBound; }

	private:
		Vec2_Physics m_LeftStartPoint, m_RightEndPoint, m_MidPoint, m_Normal;
		float m_UpperBound, m_LowerBound, m_LeftBound, m_RightBound;
		bool m_BoxBound = false;
	};

}


