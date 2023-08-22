#pragma once
#include "Physics/PhysicsObject_Spring.h"

namespace MGE {

	class PhysicsObject_SoftBody
	{
	public:
		PhysicsObject_SoftBody(int width, int height);
		~PhysicsObject_SoftBody() {}

		void OnUpdate(Timestep ts);
		void DrawSoftBody();

	private:
		std::vector<std::vector<Ref<MassPoint>>> m_MassPoints;
		std::vector<Ref<Spring>> m_Springs;
		
	};

}


