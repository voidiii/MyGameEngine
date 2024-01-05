#include "MGEpch.h"
#include "PhysicsObject_Spring.h"

namespace MGE {

	MassPoint::MassPoint(Vec2_Physics Position, int uID)
		: m_Position(Position), m_uID(uID)
	{
		m_Force = Vec2_Physics{ 0.0f, 0.0f };
		m_Velocity = Vec2_Physics{ 0.0f, 0.0f };
		m_Mass = 1.0f;
	}

	void MassPoint::DrawPhysicsObject()
	{
		Renderer2D::DrawCircle({ m_Position.x, m_Position.y, 0.0f }, Vec4{ 1.0f, 0.3f, 0.8f, 1.0f });
	}

	void MassPoint::OnUpdate(Timestep ts)
	{
		m_Force += Vec2_Physics{ 0.0f, -1.0f } * 1.0f;
		m_Velocity += (m_Force / m_Mass) * ts;
		m_Position += m_Velocity * ts;

		// reset force every frame
		m_Force = Vec2_Physics{ 0.0f, 0.0f };
	}

	Spring::Spring( float Stifiness,
				float RestLength,
				float DampFactor, 
				int uID,
				Ref<MassPoint> MassPointA,
				Ref<MassPoint> MassPointB
		) : m_Stifiness(Stifiness), m_RestLength(RestLength), m_DampFactor(DampFactor), m_uID(uID), m_MassPointA(MassPointA), m_MassPointB(MassPointB)
	{

	}

	void Spring::OnUpdate(Timestep ts)
	{
		Vec2_Physics MassPointDirection = mathter::Normalize(m_MassPointA->GetPosition() - m_MassPointB->GetPosition());
		float MassPointDistance = mathter::Length(m_MassPointA->GetPosition() - m_MassPointB->GetPosition());
		float DeltaX = MassPointDistance - m_RestLength;

		Vec2_Physics HookeanForce = m_Stifiness * DeltaX * MassPointDirection;

		m_MassPointA -> UpdateForce(-HookeanForce);
		m_MassPointB -> UpdateForce(HookeanForce);

		Vec2_Physics DampingForce = MassPointDistance * (m_MassPointA->GetVelocity() - m_MassPointB->GetVelocity()) * m_DampFactor;

		m_MassPointA->UpdateForce(-DampingForce);
		m_MassPointB->UpdateForce(DampingForce);
	}

}