#pragma once

#include "MathAlias.h"
#include "MGE.h"

namespace MGE {

	class NoneKinematicPhysicsObject
	{
		virtual ~NoneKinematicPhysicsObject() {}
		virtual void DrawNoneKinematicPhysicsObject() = 0;
		virtual Vec2_Physics GetPosition() const = 0;
	};
	
	class PhysicsObject
	{
	public:
		virtual ~PhysicsObject() {}

		virtual void DrawPhysicsObject() = 0;
		virtual Vec2_Physics GetPosition() const = 0;
		// virtual float GetGravity() const = 0;
		// virtual void SetGravity(float gravity) = 0;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void UpdateVelocity(Vec2_Physics deltaVelocity) = 0;
		virtual void ChangeVelocity(Vec2_Physics newVelocity) = 0;
		virtual void UpdatePosition(Vec2_Physics DeltaPosition) = 0;
		// virtual void SetMotionLimit(float xLimit, float yLimit) = 0;

		// virtual float GetMass() const = 0;
		// virtual Vec2 GetVelocity() const = 0;
	};

	class CirclePhyicsObject : public PhysicsObject
	{
	public:
		CirclePhyicsObject() {
			// Initialize your object here
			m_Position = Vec2_Physics{ 0.0f, 0.0f };
			m_uID = 0;
			m_Velocity = Vec2_Physics{ 0.0f, 0.0f };
		}

		CirclePhyicsObject(Vec2_Physics Position, int uID, Vec2_Physics Velocity);
		virtual ~CirclePhyicsObject();

		virtual void DrawPhysicsObject() override;
		void DebugDraw();
		inline virtual Vec2_Physics GetPosition() const { return m_Position; }

		inline Vec2 GetVelocity() const { return m_Velocity; }
		// inline float GetMass() const { return m_mass; }
		inline int GetUID() const { return m_uID; }
		inline float GetRadius() const { return m_radius; }
		inline float GetDensity() const { return m_density; }
		void SetDensity(float density) { m_density = density; }

		// inline float GetGravity() const { return m_Gravity; }
		// inline void SetGravity(float gravity) { m_Gravity = gravity; }

		virtual void OnUpdate(Timestep ts) override;
		virtual void UpdateVelocity(Vec2_Physics deltaVelocity) override;
		virtual void ChangeVelocity(Vec2_Physics newVelocity) override;
		virtual void UpdatePosition(Vec2_Physics DeltaPosition) override;

		void SetCurrentPosition(Vec2_Physics Position) { m_Position = Position; }

		void OnUpdate_Verlet(Timestep ts);
		void ApplyMotionLimit_Verlet();
		
		void SetMotionLimit(float xLimit, float yLimit);
		void ApplyMotionLimit();

		void AddSkipFrame() { m_SkipFrame++; }
		void AddSkipFrameUppperLimit() { m_SkipFrameUpperLimit++; }
		int GetSkipFrame() const { return m_SkipFrame; }
		int GetSkipFrameUppperLimit() const { return m_SkipFrameUpperLimit; }
		void ResetSkipFrame() { m_SkipFrame = 0; }
		void ResetSkipFrameUppperLimit() { m_SkipFrameUpperLimit = 4; }

		void AddPressureAcceleration(Vec2_Physics impluse) { m_PressureAcceleration += impluse; }
		void ClearPressureAcceleration() { m_PressureAcceleration = Vec2_Physics{ 0.0f, 0.0f }; }
		float GetLengthPressureAcceleration() { return mathter::Length(m_PressureAcceleration); }

	private:
		Vec2_Physics m_Position;
		Vec2_Physics m_LastPosition;
		Vec2_Physics m_Velocity = { 0.0f, 0.0f };
		Vec2_Physics m_PressureAcceleration = { 0.0f, 0.0f };
		float m_density;
		float m_radius = 0.25f;
		float m_XLimit, m_YLimit;
		int m_uID;
		int m_SkipFrame = 0;
		int m_SkipFrameUpperLimit = 4;
	};

	std::vector<Vec2_Physics> convex_hull(std::vector<Vec2_Physics> A);

}
