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
	}

	void PhysicsObject_Square::OnUpdate(Timestep ts)
	{
		std::vector<Vec2> ans = convex_hull(m_Vertices);
	}

	void PhysicsObject_Square::UpdateVelocity(Vec2 deltaVelocity)
	{
	}

	void PhysicsObject_Square::ChangeVelocity(Vec2 newVelocity)
	{
	}

	void PhysicsObject_Square::UpdatePosition(Vec2 DeltaPosition)
	{
	}

	void PhysicsObject_Square::SetMotionLimit(float xLimit, float yLimit)
	{
	}

}


