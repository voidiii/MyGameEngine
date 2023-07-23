#include "MGEpch.h"
#include "PhysicsObject.h"

namespace MGE {

	TrianglePhyicsObject::TrianglePhyicsObject(Vec2 Position, Vec4 Color)
		: m_Position(Position), m_Color(Color)
	{
	}

	TrianglePhyicsObject::~TrianglePhyicsObject()
	{
	}

	void TrianglePhyicsObject::DrawPhysicsObject()
	{
		Renderer2D::DrawQuad(m_Position, { 0.1f, 0.2f }, m_Color);
	}

	CirclePhyicsObject::CirclePhyicsObject(Vec2 Position, Vec4 Color)
		: m_Position(Position), m_Color(Color)
	{

	}

	CirclePhyicsObject::~CirclePhyicsObject()
	{
	}

	void CirclePhyicsObject::DrawPhysicsObject()
	{
		Renderer2D::DrawCircle({ m_Position.x, m_Position.y, 0.0f }, m_Color);
	}

	void CirclePhyicsObject::UpdateVelocity(Vec2 deltaVelocity)
	{
		m_Velocity += deltaVelocity;
	}

	void CirclePhyicsObject::ChangeVelocity(Vec2 newVelocity)
	{
		m_Velocity = newVelocity;
	}

	void CirclePhyicsObject::UpdatePosition(Vec2 DeltaPosition)
	{
		DeltaPosition = on_Ground ? Vec2{ DeltaPosition.x, 0.0f } : DeltaPosition;
		m_Position += DeltaPosition;
	}

	void CirclePhyicsObject::OnUpdate(Timestep ts)
	{
		//on_Ground = m_Position.y < -1.0f;

		//m_Gravity = on_Ground ? 0.0f : m_Gravity;
		//m_Velocity = on_Ground ? Vec2{ m_Velocity.x, 0.0f } : m_Velocity;

		// Vec2 FractionDirection =  -mathter::Normalize(m_Velocity);

		float FractionAccelaration =  0.01f; // fraction 

		if (std::abs(m_Position.x) > 2.0f) {
			ChangeVelocity(Vec2{ -m_Velocity.x, m_Velocity.y });
		}

		if (m_Position.y > 5.0f || m_Position.y < -1.0f) {
			ChangeVelocity(Vec2{ m_Velocity.x, -m_Velocity.y });
		}
		
		UpdateVelocity(m_Gravity * Vec2{ 0.0f, -1.0f } * 0.1f * ts /* + FractionDirection * FractionAccelaration * ts*/);

		UpdatePosition(m_Velocity * ts);
	}

}