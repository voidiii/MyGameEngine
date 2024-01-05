#pragma once
#include "PhysicsObject.h"
#include "MGE/Core/ProfilingTimer.h"

#include <unordered_map>
#include <utility>
#include "nmmintrin.h" //SSE4.2, 128 bit operations (4 floats)
#include "immintrin.h" //AVX2, 256 bit operations (8 floats)
#include "MGE/Core/ThreadPool.h"

namespace MGE {

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	struct Grid
	{
		Grid(float x, float y)
			: x(x), y(y)
		{
		}

		float x, y;
		std::vector<int> object_id;
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
		~PhysicsScene() {
			m_ThreadPool.Stop();
		}

		void OnUpdate(Timestep ts);
		
		inline CirclePhyicsObject* GetPhysicsObjects(int index) { return m_PhysicsObjects[index]; }


		void FindCollisions();
		void FindCollisions_mutithread(int start_X, int start_Y);
		void FindCollisions_mutithread_Call();
		void FindCollisions_BrutalForce();
		void FindCollisions_GridHandling(int i, int j);

		void ElasticCollisions(CirclePhyicsObject* i, CirclePhyicsObject* j);
		void ElasticCollisions_Verlet(CirclePhyicsObject* i, CirclePhyicsObject* j);

		inline int GetNumberOfObjects() { return m_NumberOfObjects; }
		inline void AddNumberOfObjects() { m_NumberOfObjects++;  }
		void CreateObjects(int& count);
		void GridManage();
		void SetUpGrid();

		inline std::vector<ProfileResult> GetProfileResult() const { return m_ProfileResults; }

	private:
		int m_NumberOfObjects;
		std::vector<CirclePhyicsObject*> m_PhysicsObjects;
		std::vector<std::vector<Ref<Grid>>> m_Grid;

		// std::unordered_map<int, Scope<CoordinatesWithUID>> m_CoordinatesWithUID;
		std::vector<CirclePhyicsObject> m_CirclePhyicsObjectContainer;

		std::unordered_set<int> m_CollisionUIDs;
		
		float m_SceneWidth, m_SceneHeight;
		int m_int_width, m_int_height;

		std::vector<ProfileResult> m_ProfileResults;

		ThreadPool m_ThreadPool;
	};

}


