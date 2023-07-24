#pragma once
#include "PhysicsObject.h"

namespace MGE {

	struct Grid
	{
		Grid(float x, float y)
			: x(x), y(y)
		{
		}
		
		float x, y;
		std::vector<Ref<MGE::CirclePhyicsObject>> GridObjects;
	};

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
		void GridManage();

	private:
		int m_NumberOfObjects;
		std::vector<Ref<MGE::CirclePhyicsObject>> m_PhysicsObjects;
		std::vector<std::vector<Ref<Grid>>> m_Grid;
		float m_SceneWidth, m_SceneHeight;
	};

}


