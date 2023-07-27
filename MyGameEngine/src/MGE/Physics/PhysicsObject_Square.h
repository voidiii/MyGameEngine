#pragma once
#include "PhysicsObject.h"

namespace MGE {

	class PhysicsObject_Square : public PhysicsObject
	{
	public:
		PhysicsObject_Square(Vec2 Position, Vec4 Color, int uID);
		virtual ~PhysicsObject_Square();

		virtual void DrawPhysicsObject() override;
		inline virtual Vec2 GetPosition() const { return m_Position; }

		inline Vec2 GetVelocity() const { return m_Velocity; }
		inline float GetMass() const { return m_Mass; }
		inline float GetUID() const { return m_UID; }
		inline float GetLength() const { return m_Length; }

		inline float GetGravity() const { return m_Gravity; }
		inline void SetGravity(float gravity) { m_Gravity = gravity; }

		virtual void OnUpdate(Timestep ts) override;
		virtual void UpdateVelocity(Vec2 deltaVelocity) override;
		virtual void ChangeVelocity(Vec2 newVelocity) override;
		virtual void UpdatePosition(Vec2 DeltaPosition) override;

		void SetMotionLimit(float xLimit, float yLimit);

	private:
		Vec2 m_Position;
		Vec4 m_Color;
		Vec2 m_MassCenter;
		Vec2 m_Velocity = { 0.0f, 0.0f };
		std::vector<Vec2> m_Vertices;
		float m_Length = 0.1f;
		float m_Mass = 1.0f;
		bool on_Ground = false;
		float m_Gravity = 9.8f;
		float m_XLimit, m_YLimit;
		int m_UID;
	};

}


