#include "MGEpch.h"
#include "PhysicsObject_Square.h"

namespace MGE {

	PhysicsObject_Square::PhysicsObject_Square(Vec2 Position, Vec4 Color, int uID)
		:	m_Position(Position), m_Color(Color), m_UID(uID)
	{
		m_Vertices.reserve(4);

		m_Vertices.push_back(Vec2(m_Length, m_Length));
		m_Vertices.push_back(Vec2(-m_Length, m_Length));
		m_Vertices.push_back(Vec2(m_Length, -m_Length));
		m_Vertices.push_back(Vec2(-m_Length, -m_Length));
		
	}

	PhysicsObject_Square::~PhysicsObject_Square()
	{
	}

	void PhysicsObject_Square::DrawPhysicsObject()
	{
		Renderer2D::DrawQuad(m_Position, Vec2(m_Length, m_Length), m_Color);
	}

	void PhysicsObject_Square::OnUpdate(Timestep ts)
	{
		on_Ground = m_Position.y < -m_YLimit;
		m_Gravity = on_Ground ? 0.0f : 9.8f;

		Vec2 FractionDirection = mathter::Length(m_Velocity) > 0.0f ? -mathter::Normalize(m_Velocity) : Vec2{ 0.0f, 0.0f };

		float FractionAccelaration = 2.0f; // fraction 

		if (std::abs(m_Position.x) > m_XLimit) {
			ChangeVelocity(Vec2{ -0.9f * m_Velocity.x, m_Velocity.y });
		}

		if (m_Position.y < -m_YLimit) {
			ChangeVelocity(Vec2{ m_Velocity.x, 0.9f * (-m_Velocity.y) });
			UpdatePosition(Vec2{ 0.0f , -m_YLimit - m_Position.y });
		}

		UpdateVelocity(m_Gravity * Vec2{ 0.0f, -1.0f } *ts + FractionDirection * FractionAccelaration * ts);
		UpdatePosition(m_Velocity * ts);
	}

	void PhysicsObject_Square::UpdateVelocity(Vec2 deltaVelocity)
	{
		m_Velocity += deltaVelocity;
	}

	void PhysicsObject_Square::ChangeVelocity(Vec2 newVelocity)
	{
		m_Velocity = newVelocity;
	}

	void PhysicsObject_Square::UpdatePosition(Vec2 DeltaPosition)
	{
		m_Position += DeltaPosition;
	}

	void PhysicsObject_Square::SetMotionLimit(float xLimit, float yLimit)
	{
		m_XLimit = xLimit;
		m_YLimit = yLimit;
	}

}


