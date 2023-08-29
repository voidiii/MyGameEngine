#include "MGEpch.h"
#include "Scene_SpringAndSoftbody.h"

namespace MGE {

	Scene_SpringAndSoftbody::Scene_SpringAndSoftbody(float height, float width, int numberOfObjects)
		: m_SceneHeight(height), m_SceneWidth(width), m_NumberOfObjects(numberOfObjects)
	{
		// m_MassPoints.push_back(CreateRef<MassPoint>(Vec2_Physics{ 0.0f, 3.0f }, 0));
		// m_MassPoints.push_back(CreateRef<MassPoint>(Vec2_Physics{ 0.0f, 6.0f }, 1));
		// 
		// m_SpringContainer.push_back(CreateRef<Spring>(1.0f, 1.0f, 1.0f, 0, m_MassPoints[0], m_MassPoints[1]));

		m_SoftBody = CreateRef<PhysicsObject_SoftBody>(3, 3);

		m_StaticLines.push_back(CreateRef<StaticObject_Line>(Vec2_Physics{ -10.0f, 5.0f }, Vec2_Physics{ 1.0f,-5.0f }, false));
		m_StaticLines.push_back(CreateRef<StaticObject_Line>(Vec2_Physics{ -2.0f, 2.0f }, Vec2_Physics{ 3.0f,4.0f }, false));
		m_StaticLines.push_back(CreateRef<StaticObject_Line>(Vec2_Physics{ -10.0f, -5.0f }, Vec2_Physics{ 10.0f, -5.0f }, true));
		//m_StaticLines.push_back(CreateRef<StaticObject_Line>(Vec2_Physics{ 10.0f, 10.0f }, Vec2_Physics{ 10.0f, 0.0f }, true));
	}

	void Scene_SpringAndSoftbody::OnUpdate(Timestep ts)
	{
		m_SoftBody->OnUpdate(ts);

		for (auto staticLine : m_StaticLines)
		{
			for (auto massPoints : m_SoftBody->GetMassPoints())
			{
				for (auto massPoint : massPoints)
				{
					if(!staticLine->IsBoxBound() && (massPoint->GetPosition().x < staticLine->GetLeftBound()
						|| massPoint->GetPosition().x > staticLine->GetRightBound()
						|| massPoint->GetPosition().y < staticLine->GetLowerBound()
						|| massPoint->GetPosition().y > staticLine->GetUpperBound()
					))
					{
						continue;
					}
				
					if (mathter::Dot(massPoint->GetPosition(), staticLine->GetNormal()) < (mathter::Dot(staticLine->GetMidPoint(), staticLine->GetNormal()) + 0.5f)
					&& mathter::Dot(massPoint->GetPosition(), staticLine->GetNormal()) > (mathter::Dot(staticLine->GetMidPoint(), staticLine->GetNormal()) - 0.5f)
					)
					{
						staticLine->ApplySupport(ts, massPoint);
					}
				}
			}
		}

		m_SoftBody->DrawSoftBody();
	}

	void Scene_SpringAndSoftbody::FindCollisions()
	{
	}

	void Scene_SpringAndSoftbody::ElasticCollisions(CirclePhyicsObject* i, CirclePhyicsObject* j)
	{
	}

}
