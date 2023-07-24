#include "MGEpch.h"
#include "PhysicsObject.h"

namespace MGE {

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
		// DeltaPosition = on_Ground ? Vec2{ DeltaPosition.x, 0.0f } : DeltaPosition;
		m_Position += DeltaPosition;
	}

	void CirclePhyicsObject::OnUpdate(Timestep ts)
	{
		on_Ground = m_Position.y < -m_YLimit;
		// m_Position.y = on_Ground ? -m_YLimit : m_Position.y;

		m_Gravity = on_Ground ? 0.0f : 9.8f;
		// m_Velocity = on_Ground ? Vec2{ m_Velocity.x, 0.0f } : m_Velocity;

		Vec2 FractionDirection = mathter::Length(m_Velocity) > 0.0f ? -mathter::Normalize(m_Velocity) : Vec2{0.0f, 0.0f};

		float FractionAccelaration =  2.0f; // fraction 

		if (std::abs(m_Position.x) > m_XLimit) {
			ChangeVelocity(Vec2{ -0.9f * m_Velocity.x, m_Velocity.y });
		}

		if (m_Position.y < -m_YLimit) {
			ChangeVelocity(Vec2{ m_Velocity.x, 0.9f * (-m_Velocity.y) });
			UpdatePosition(Vec2{ 0.0f , -m_YLimit - m_Position.y });
		}
		
		UpdateVelocity(m_Gravity * Vec2{ 0.0f, -1.0f } * ts + FractionDirection * FractionAccelaration * ts);

		UpdatePosition(m_Velocity * ts);
	}

	void CirclePhyicsObject::SetMotionLimit(float xLimit, float yLimit)
	{
		m_XLimit = xLimit;
		m_YLimit = yLimit;
	}

}