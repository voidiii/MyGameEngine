#include "MGEpch.h"
#include "PhysicsObject_Square.h"

namespace MGE {

	PhysicsObject_Square::PhysicsObject_Square(Vec2_Physics Position, Vec4 Color, int uID)
		: m_Position(Position), m_Color(Color), m_UID(uID), m_LastPosition(Vec2_Physics{ Position.x , Position.y })
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
		Vec2_Physics velocity = m_Position - m_LastPosition;

		if (velocity.x > 0.0f)
		{
			printf("UID:%d, velocity.x:%f \n", m_UID, velocity.x);
		}

		m_LastPosition = m_Position;
		m_Position += velocity + (Vec2_Physics{ 0.0f, -1.0f } - velocity) * ts * ts;
		ApplyMotionLimit_Verlet();
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

	void PhysicsObject_Square::ApplyMotionLimit_Verlet()
	{
		if (m_Position.x > m_XLimit) {
			m_Position.x = m_XLimit + (m_XLimit - m_Position.x);
		}

		if (m_Position.x < -m_XLimit) {
			m_Position.x = -m_XLimit + (-m_XLimit - m_Position.x);
		}

		if (m_Position.y < -m_YLimit) {
			m_Position.y = -m_YLimit + (-m_YLimit - m_Position.y);
		}

		if (m_Position.y > m_YLimit) {
			m_Position.y = m_YLimit + (m_YLimit - m_Position.y);
		}
	}

}


