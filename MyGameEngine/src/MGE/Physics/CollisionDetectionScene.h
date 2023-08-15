#pragma once
#include <map>
#include "PhysicsObject_Square.h"
#include "Collision.h"

namespace MGE {

	class CollisionDetectionScene
	{
	public:
		CollisionDetectionScene(float height, float width, int numberOfObjects);
		~CollisionDetectionScene() {}

		void OnUpdate(Timestep ts);

		inline Ref<MGE::PhysicsObject_Square> GetPhysicsObjects(int index) { return m_PhysicsObjects[index]; }


		bool FindCollisions(Ref<MGE::PhysicsObject_Square> Object_A, Ref<MGE::PhysicsObject_Square> Object_B, std::vector<Vec2_Physics>& Result_Convexhull);
		void ResolveCollision(Ref<MGE::PhysicsObject_Square> i, Ref<MGE::PhysicsObject_Square> j, std::vector<Vec2_Physics>& Result_Convexhull);
		inline int GetNumberOfObjects() { return m_NumberOfObjects; }

		void BroadPhase();

		void CreateObjects(int& count);

	private:
		int m_NumberOfObjects;
		std::vector<Ref<MGE::PhysicsObject_Square>> m_PhysicsObjects;

		float m_SceneWidth, m_SceneHeight;

		std::map<CollisionKey, Arbiter> m_CollisionContainer;
	};
}

