#pragma once
#include "MathAlias.h"
#include "MGE/Physics/PhysicsObject_Square.h"

namespace MGE {

	union FeaturePair
	{
		struct Edges
		{
			char inEdge1;
			char outEdge1;
			char inEdge2;
			char outEdge2;
		} e;
		int value;
	};
	
	struct Collision
	{
		Collision() : Pn(0.0f), Pt(0.0f), Pnb(0.0f) {}
		
		Vec2_Physics position;
		Vec2_Physics normal;
		Vec2_Physics r1, r2;
		float separation;
		float Pn;	// accumulated normal impulse
		float Pt;	// accumulated tangent impulse
		float Pnb;	// accumulated normal impulse for position bias
		float massNormal, massTangent;
		float bias;
		FeaturePair feature;
	};

	class CollisionKey
	{
	public:
		CollisionKey(Ref<PhysicsObject_Square> body_1, Ref<PhysicsObject_Square> body_2) {
			m_CollisionKey = body_1->GetUID() > body_2->GetUID() ? body_1->GetUID() : body_2->GetUID();
		}

		inline int Get_CollisionKey() { return m_CollisionKey; }

	private:
		int m_CollisionKey;
	};

	struct Arbiter
	{
		enum { MAX_POINTS = 2 };

		Arbiter(Ref<PhysicsObject_Square> b1, Ref<PhysicsObject_Square> b2);

		void Update(Collision* contacts, int numContacts);

		void PreStep(float inv_dt);
		void ApplyImpulse();

		Collision CollisionPoints[MAX_POINTS];
		int numContacts;

		Ref<PhysicsObject_Square> body1;
		Ref<PhysicsObject_Square> body2;

		// Combined friction
		// float friction;
	};
}

