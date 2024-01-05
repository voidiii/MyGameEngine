#pragma once
#include "PhysicsObject.h"

namespace MGE {

	// Box vertex and edge numbering:
	//
	//        ^ y
	//        |
	//        e1
	//   v2 ------ v1
	//    |        |
	// e2 |        | e4  --> x
	//    |        |
	//   v3 ------ v4
	//        e3

	enum Axis
	{
		FACE_A_X,
		FACE_A_Y,
		FACE_B_X,
		FACE_B_Y
	};

	enum EdgeNumbers
	{
		NO_EDGE = 0,
		EDGE1,
		EDGE2,
		EDGE3,
		EDGE4
	};

	class PhysicsObject_Square : public PhysicsObject
	{
	public:
		PhysicsObject_Square(Vec2_Physics Position, Vec4 Color, int uID);
		virtual ~PhysicsObject_Square();

		virtual void DrawPhysicsObject() override;
		inline virtual Vec2_Physics GetPosition() const { return m_Position; }

		inline Vec2 GetVelocity() const { return m_Velocity; }
		// inline float GetMass() const { return m_Mass; }
		inline float GetUID() const { return m_UID; }
		inline float GetLength() const { return m_Length; }

		inline float GetGravity() const { return m_Gravity; }
		inline void SetGravity(float gravity) { m_Gravity = gravity; }
		inline void SetColor(Vec4 color) { m_Color = color; }


		virtual void OnUpdate(Timestep ts) override;


		virtual void UpdateVelocity(Vec2_Physics deltaVelocity) override;
		virtual void ChangeVelocity(Vec2_Physics newVelocity) override;
		virtual void UpdatePosition(Vec2_Physics DeltaPosition) override;

		void SetMotionLimit(float xLimit, float yLimit);

		inline int GetNumOfVertices() { return m_numOfVertices;  }
		inline Vec2 GetVertices(int index) { return m_Vertices[index]; }

		void ApplyMotionLimit_Verlet();

		void AddForce(const Vec2& f)
		{
			m_Force += f;
		}

		void SetCurrentPosition(Vec2_Physics Position) { m_Position = Position; }

	private:
		Vec2_Physics m_Position;
		Vec2_Physics m_LastPosition;
		Vec4 m_Color;
		Vec2_Physics m_MassCenter;
		Vec2_Physics m_Velocity = { 0.0f, 0.0f };
		std::vector<Vec2> m_Vertices;
		float m_Length = 0.1f;
		// float m_Mass = 1.0f;
		bool on_Ground = false;
		float m_Gravity = 9.8f;
		float m_XLimit, m_YLimit;

		Vec2_Physics m_Force;
		float m_Torque;

		float m_AngularVelocity = 0.0f;
		float m_Angle = 0.0f;

		int m_UID;
		int m_numOfVertices = 4;
	};

}


