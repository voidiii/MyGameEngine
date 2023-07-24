#pragma once
#include "PhysicsObject.h"

namespace MGE {

	class PhysicsScene
	{
	public:
		PhysicsScene(float height, float width, int numberOfObjects);
		~PhysicsScene() {}

		void OnUpdate(Timestep ts);
		
		inline Ref<MGE::CirclePhyicsObject> GetPhysicsObjects(int index) { return m_PhysicsObjects[index]; }


		void FindCollisions();
		void ElasticCollisions(Ref<MGE::CirclePhyicsObject> i, Ref<MGE::CirclePhyicsObject> j);
		inline int GetNumberOfObjects() { return m_NumberOfObjects; }
		void CreateObjects();

	private:
		int m_NumberOfObjects;
		std::vector<Ref<MGE::CirclePhyicsObject>> m_PhysicsObjects;
		float m_SceneWidth, m_SceneHeight;
	};

}


