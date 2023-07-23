#pragma once
#include "PhysicsObject.h"

namespace MGE {

	class PhysicsScene
	{
	public:
		PhysicsScene();
		~PhysicsScene() {}

		void OnUpdate(Timestep ts);
		
		inline Ref<MGE::CirclePhyicsObject> GetPhysicsObjects(int index) { return m_PhysicsObjects[index]; }


		void FindCollisions();
		void ElasticCollisions(Ref<MGE::CirclePhyicsObject> i, Ref<MGE::CirclePhyicsObject> j);

	private:
		Ref<MGE::CirclePhyicsObject> m_PhysicsObjects[10];
	};

}


