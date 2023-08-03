#include "MGEpch.h"
#include "PhysicsObject_Square.h"

namespace MGE {

	PhysicsObject_Square::PhysicsObject_Square(Vec2_Physics Position, Vec4 Color, int uID)
		: m_Position(Position), m_Color(Color), m_UID(uID)
	{
		m_Vertices.reserve(4);

		m_Vertices.push_back(Vec2(m_Length, m_Length));
		m_Vertices.push_back(Vec2(-m_Length, m_Length));
		m_Vertices.push_back(Vec2(-m_Length, -m_Length));
		m_Vertices.push_back(Vec2(m_Length, -m_Length));
		
	}

	PhysicsObject_Square::~PhysicsObject_Square()
	{
	}

	void PhysicsObject_Square::DrawPhysicsObject()
	{
		Mat22 transform = mathter::Rotation(m_Angle);
		std::vector<Vec3> vertices;
		for (int i = 0; i < 4; i++)
		{
			vertices.emplace_back(Vec3(
				m_Vertices[i].x * transform(0, 0) + m_Vertices[i].y * transform(0, 1),
				m_Vertices[i].x * transform(1, 0) + m_Vertices[i].y * transform(1, 1),
					0.0f));
		}
		Renderer2D::DrawQuad(m_Position, m_Color, vertices);
	}

	void PhysicsObject_Square::OnUpdate(Timestep ts)
	{
		on_Ground = m_Position.y < -m_YLimit;
		m_Gravity = on_Ground ? 0.0f : 9.8f;

		Vec2_Physics FractionDirection = mathter::Length(m_Velocity) > 0.0f ? -mathter::Normalize(m_Velocity) : Vec2_Physics{ 0.0f, 0.0f };

		float FractionAccelaration = 2.0f; // fraction 

		if (std::abs(m_Position.x) > m_XLimit) {
			ChangeVelocity(Vec2_Physics{ -0.9f * m_Velocity.x, m_Velocity.y });
		}

		if (m_Position.y < -m_YLimit) {
			ChangeVelocity(Vec2_Physics{ m_Velocity.x, 0.9f * (-m_Velocity.y) });
			UpdatePosition(Vec2_Physics{ 0.0f , -m_YLimit - m_Position.y });
		}

		UpdateVelocity(m_Gravity * Vec2_Physics{ 0.0f, -1.0f } * ts + FractionDirection * FractionAccelaration * ts);
		UpdatePosition(m_Velocity * ts);
	}

	void PhysicsObject_Square::UpdateVelocity(Vec2_Physics deltaVelocity)
	{
		m_Velocity += deltaVelocity;
	}

	void PhysicsObject_Square::ChangeVelocity(Vec2_Physics newVelocity)
	{
		m_Velocity = newVelocity;
	}

	void PhysicsObject_Square::UpdatePosition(Vec2_Physics DeltaPosition)
	{
		m_Position += DeltaPosition;
	}

	void PhysicsObject_Square::SetMotionLimit(float xLimit, float yLimit)
	{
		m_XLimit = xLimit;
		m_YLimit = yLimit;
	}

}


