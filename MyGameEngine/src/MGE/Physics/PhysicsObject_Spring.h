#pragma once
#include "PhysicsObject.h"

namespace MGE {

	class MassPoint : PhysicsObject
	{
	public:
		MassPoint(Vec2_Physics Position, int uID);
		virtual ~MassPoint() {}
		virtual void DrawPhysicsObject();

		virtual void OnUpdate(Timestep ts) override;

		inline virtual Vec2_Physics GetPosition() const { return m_Position; }
		inline virtual void UpdateVelocity(Vec2_Physics deltaVelocity) { m_Velocity += deltaVelocity; }
		inline virtual void ChangeVelocity(Vec2_Physics newVelocity) { m_Velocity = newVelocity; }
		inline virtual void UpdatePosition(Vec2_Physics deltaPosition) { m_Position += deltaPosition; }

		inline float GetMass() { return m_Mass; }
		inline Vec2_Physics GetVelocity() const { return m_Velocity; }
		
		inline void UpdateForce(Vec2_Physics deltaForce) { m_Force += deltaForce; }
		inline float GetDiameter() { return m_Diameter; }

	private:
		Vec2_Physics m_Position;
		Vec2_Physics m_Velocity;
		Vec2_Physics m_Force;
		float m_Mass;
		float m_Diameter = 1.0f;

		int m_uID;
	};
	
	class Spring
	{
	public:
		Spring( float Stifiness,
				float RestLength,
				float DampFactor, 
				int uID,
				Ref<MassPoint> MassPointA,
				Ref<MassPoint> MassPointB
		);

		/* In Spring Update function we need to change the force on each mass point, the rest will leave to mass point's update function */
		void OnUpdate(Timestep ts);

	private:
		Ref<MassPoint> m_MassPointA;
		Ref<MassPoint> m_MassPointB;

		float m_Stifiness;
		float m_RestLength;
		float m_DampFactor;

		int m_uID;
	};

}

