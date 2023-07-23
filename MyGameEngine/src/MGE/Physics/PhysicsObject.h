#pragma once

#include "MathAlias.h"
#include "MGE.h"

namespace MGE {

	class PhysicsObject
	{
	public:
		virtual ~PhysicsObject() {}

		virtual void DrawPhysicsObject() = 0;
		virtual Vec2 GetPosition() const = 0;
		virtual float GetGravity() const = 0;
		virtual void SetGravity(float gravity) = 0;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void UpdateVelocity(Vec2 deltaVelocity) = 0;
		virtual void ChangeVelocity(Vec2 newVelocity) = 0;
		virtual void UpdatePosition(Vec2 DeltaPosition) = 0;

		virtual float GetMass() const = 0;
		virtual Vec2 GetVelocity() const = 0;
	};

	class TrianglePhyicsObject : public PhysicsObject
	{
	public:
		TrianglePhyicsObject(Vec2 Position, Vec4 Color);
		virtual ~TrianglePhyicsObject();

		virtual void DrawPhysicsObject() override;
		inline virtual Vec2 GetMassCenter() const { return m_MassCenter; }

		inline virtual float GetGravity() const { return m_Gravity; }
		inline virtual void SetGravity(float gravity) { m_Gravity = gravity; }

	private:
		Vec2 m_Position;
		Vec4 m_Color;
		Vec2 m_MassCenter;
		float m_Gravity = 9.8f;
	};

	class CirclePhyicsObject : public PhysicsObject
	{
	public:
		CirclePhyicsObject(Vec2 Position, Vec4 Color);
		virtual ~CirclePhyicsObject();

		virtual void DrawPhysicsObject() override;
		inline virtual Vec2 GetPosition() const { return m_Position; }
		inline virtual Vec2 GetVelocity() const { return m_Velocity; }
		inline virtual float GetMass() const { return m_mass; }

		inline virtual float GetGravity() const { return m_Gravity; }
		inline virtual void SetGravity(float gravity) { m_Gravity = gravity; }

		virtual void OnUpdate(Timestep ts) override;
		virtual void UpdateVelocity(Vec2 deltaVelocity) override;
		virtual void ChangeVelocity(Vec2 newVelocity) override;
		virtual void UpdatePosition(Vec2 DeltaPosition) override;

	private:
		Vec2 m_Position;
		Vec4 m_Color;
		Vec2 m_MassCenter;
		Vec2 m_Velocity = { 0.0f, 0.0f };
		float m_radius = 0.1f;
		float m_mass = 1.0f;
		bool on_Ground = false;
		float m_Gravity = 9.8f;
	};
}
