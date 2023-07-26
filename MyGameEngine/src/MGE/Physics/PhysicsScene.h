#pragma once
#include "PhysicsObject.h"
#include <unordered_map>
#include <utility>

namespace MGE {

	struct Grid
	{
		Grid(float x, float y, Ref<std::unordered_map<int, Ref<CirclePhyicsObject>>> g)
			: x(x), y(y), GridObjects(g)
		{
		}

		float x, y;
		Ref<std::unordered_map<int, Ref<CirclePhyicsObject>>> GridObjects;
	};

	struct CoordinatesWithUID
	{
		CoordinatesWithUID(float x, float y, int uID)
			: x(x), y(y), uID(uID)
		{
		}

		float x, y;
		int uID;
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
		void CreateObjects(int& count);
		void GridManage();
		void SetUpGrid();

	private:
		int m_NumberOfObjects;
		std::vector<Ref<MGE::CirclePhyicsObject>> m_PhysicsObjects;
		std::vector<std::vector<Ref<Grid>>> m_Grid;

		std::unordered_map<int, Ref<CoordinatesWithUID>> m_CoordinatesWithUID;
		std::unordered_map<int, Ref<MGE::CirclePhyicsObject>> m_CirclePhyicsObjectContainer;

		std::unordered_set<int> m_CollisionUIDs;
		
		float m_SceneWidth, m_SceneHeight;
	};

}


