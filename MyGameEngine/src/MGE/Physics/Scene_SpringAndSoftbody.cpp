#include "MGEpch.h"
#include "Scene_SpringAndSoftbody.h"

namespace MGE {

	Scene_SpringAndSoftbody::Scene_SpringAndSoftbody(float height, float width, int numberOfObjects)
		: m_SceneHeight(height), m_SceneWidth(width), m_NumberOfObjects(numberOfObjects)
	{
		m_MassPoints.push_back(CreateRef<MassPoint>(Vec2_Physics{ 0.0f, 3.0f }, 0));
		m_MassPoints.push_back(CreateRef<MassPoint>(Vec2_Physics{ 0.0f, 6.0f }, 1));

		m_SpringContainer.push_back(CreateRef<Spring>(1.0f, 1.0f, 1.0f, 0, m_MassPoints[0], m_MassPoints[1]));

		m_StaticLines.push_back(CreateRef<StaticObject_Line>(Vec2_Physics{ -2.0f, 2.0f }, Vec2_Physics{ 1.0f, 0.0f }, false));
		m_StaticLines.push_back(CreateRef<StaticObject_Line>(Vec2_Physics{ -10.0f, 0.0f }, Vec2_Physics{ 10.0f, 0.0f }, true));
		m_StaticLines.push_back(CreateRef<StaticObject_Line>(Vec2_Physics{ 10.0f, 10.0f }, Vec2_Physics{ 10.0f, 0.0f }, true));
	}

	void Scene_SpringAndSoftbody::OnUpdate(Timestep ts)
	{
		for(auto spring : m_SpringContainer)
		{
			spring->OnUpdate(ts);
		}

		for(auto massPoint : m_MassPoints)
		{
			massPoint->OnUpdate(ts);
		}

		for (auto staticLine : m_StaticLines)
		{
			for (auto massPoint : m_MassPoints)
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

		for(auto massPoint : m_MassPoints)
		{
			massPoint->DrawPhysicsObject();
		}
	}

	void Scene_SpringAndSoftbody::FindCollisions()
	{
	}

	void Scene_SpringAndSoftbody::ElasticCollisions(CirclePhyicsObject* i, CirclePhyicsObject* j)
	{
	}

}
