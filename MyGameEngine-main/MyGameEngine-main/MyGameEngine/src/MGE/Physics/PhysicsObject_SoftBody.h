#pragma once
#include "PhysicsObject_Spring.h"

namespace MGE {

	class PhysicsObject_SoftBody
	{
	public:
		PhysicsObject_SoftBody(int width, int height);
		~PhysicsObject_SoftBody() {}

		void OnUpdate(Timestep ts);
		void DrawSoftBody();
		void ResolveSlefCollision();

		inline std::vector<std::vector<Ref<MassPoint>>> GetMassPoints() { return m_MassPoints; }

	private:
		std::vector<std::vector<Ref<MassPoint>>> m_MassPoints;
		std::vector<Ref<Spring>> m_Springs;
	};

}


