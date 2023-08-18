#pragma once
#include "MGE/Core/ProfilingTimer.h"
#include <unordered_map>
#include <utility>
#include "PhysicsObject_Spring.h"
#include "StaticObject.h"

namespace MGE {

	class Scene_SpringAndSoftbody
	{
	public:
		Scene_SpringAndSoftbody(float height, float width, int numberOfObjects);
		~Scene_SpringAndSoftbody() {}

		/* go through all the spring's update function */
		void OnUpdate(Timestep ts);

		/* see if there are collisions between mass point, if do, resolve them */
		void FindCollisions();
		void ElasticCollisions(CirclePhyicsObject* i, CirclePhyicsObject* j);

	private:
		std::vector<Ref<Spring>> m_SpringContainer;
		std::vector<Ref<MassPoint>> m_MassPoints; // the scene will do the render call 
		std::vector<Ref<StaticObject_Line>> m_StaticLines;
		float m_SceneWidth, m_SceneHeight;

		int m_NumberOfObjects;
	};

}

