#pragma once

#include "MathAlias.h"
#include "MGE.h"

namespace MGE {

	class PhysicsObject
	{
	public:
		virtual ~PhysicsObject() {}

		virtual void DrawPhysicsObject() = 0;
		virtual Vec3 GetMassCenter() const = 0;
		virtual float GetGravity() const = 0;
		virtual void SetGravity(float gravity) = 0;
	};

	class TrianglePhyicsObject : public PhysicsObject
	{
	public:
		TrianglePhyicsObject(Vec2 Position, Vec4 Color);
		virtual ~TrianglePhyicsObject();

		virtual void DrawPhysicsObject() override;
		inline virtual Vec3 GetMassCenter() const { return m_MassCenter; }

		inline virtual float GetGravity() const { return m_Gravity; }
		inline virtual void SetGravity(float gravity) { m_Gravity = gravity; }

	private:
		Vec2 m_Position;
		Vec4 m_Color;
		Vec3 m_MassCenter;
		float m_Gravity = 9.8f;
	};
}
