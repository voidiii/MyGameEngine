#include "MGEpch.h"
#include "Collision.h"

namespace MGE {

	int Collide(Collision* collision_points, Ref<PhysicsObject_Square> b1, Ref<PhysicsObject_Square> b2)
	{
		
	}

	Arbiter::Arbiter(Ref<PhysicsObject_Square> b1, Ref<PhysicsObject_Square> b2)
	{
		if (b1->GetUID() < b2->GetUID())
		{
			body1 = b1;
			body2 = b2;
		}
		else
		{
			body1 = b2;
			body2 = b1;
		}

		numContacts = Collide(CollisionPoints, body1, body2);

		// friction = sqrtf(body1->friction * body2->friction);
	}

}