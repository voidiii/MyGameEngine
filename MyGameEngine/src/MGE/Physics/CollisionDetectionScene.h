#pragma once
#include "PhysicsObject_Square.h"

namespace MGE {

	class CollisionDetectionScene
	{
	public:
		CollisionDetectionScene(float height, float width, int numberOfObjects);
		~CollisionDetectionScene() {}

		void OnUpdate(Timestep ts);

		inline Ref<MGE::PhysicsObject_Square> GetPhysicsObjects(int index) { return m_PhysicsObjects[index]; }


		void FindCollisions(Ref<MGE::PhysicsObject_Square> Object_A, Ref<MGE::PhysicsObject_Square> Object_B);
		inline int GetNumberOfObjects() { return m_NumberOfObjects; }

	private:
		int m_NumberOfObjects;
		std::unordered_map<int, Ref<MGE::PhysicsObject_Square>> m_PhysicsObjects;

		float m_SceneWidth, m_SceneHeight;
	};
	};

}

