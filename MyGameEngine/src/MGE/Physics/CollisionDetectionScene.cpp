#include "MGEpch.h"
#include "CollisionDetectionScene.h"

namespace MGE {

	CollisionDetectionScene::CollisionDetectionScene(float height, float width, int numberOfObjects)
	{
	}

	void CollisionDetectionScene::OnUpdate(Timestep ts)
	{
	}

	void CollisionDetectionScene::FindCollisions(Ref<MGE::PhysicsObject_Square> Object_A, Ref<MGE::PhysicsObject_Square> Object_B)
	{
		std::vector<Vec2> Minkowski_sub;
		std::vector<Vec2> Result_Convexhull;
		Minkowski_sub.reserve(Object_A->GetNumOfVertices() * Object_B->GetNumOfVertices());

		for (int i = 0; i < Object_A->GetNumOfVertices(); i++)
		{
			for (int j = 0; j < Object_B->GetNumOfVertices(); j++)
			{
				Minkowski_sub.push_back(Object_A->GetVertices(i) - Object_B->GetVertices(j));
			}
		}

		Result_Convexhull = convex_hull(Minkowski_sub);
	}

}
