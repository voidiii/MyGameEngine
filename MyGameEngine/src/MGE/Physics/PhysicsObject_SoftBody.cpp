#include "MGEpch.h"
#include "PhysicsObject_SoftBody.h"

MGE::PhysicsObject_SoftBody::PhysicsObject_SoftBody(int width, int height)
{
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			m_MassPoints[i].push_back(CreateRef<MassPoint>(MGE::Vec2_Physics(i, j), i * i + j));
		}
	}

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i == 0 && j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i + 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i + 1][j + 1]));
				continue;
			}

			if (i == width - 1 && j == height - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i - 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i - 1][j - 1]));
				continue;
			}

			if (i == 0 && j == height - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i + 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i + 1][j - 1]));
				continue;
			}

			if (i == width - 1 && j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i - 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i - 1][j + 1]));
				continue;
			}

			if (i == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i + 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i + 1][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i + 1][j - 1]));
				continue;
			}

			if (i == width - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i - 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i - 1][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i - 1][j + 1]));
				continue;
			}

			if (j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i + 1][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i + 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i - 1][j + 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i - 1][j]));
				continue;
			}

			if (j == height - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i + 1][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i + 1][j]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i - 1][j - 1]));
				m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i - 1][j]));
				continue;
			}

			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j + 1]));
			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i + 1][j]));
			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i][j - 1]));
			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.0f, 0.1f, i * i + j, m_MassPoints[i - 1][j]));
			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i - 1][j - 1]));
			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i + 1][j + 1]));
			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i + 1][j - 1]));
			m_Springs.push_back(CreateRef<Spring>(1.0f, 1.414f, 0.1f, i * i + j, m_MassPoints[i - 1][j + 1]));

		}
	}
}

void MGE::PhysicsObject_SoftBody::OnUpdate(Timestep ts)
{
	for (auto spring : m_Springs)
	{
		spring->OnUpdate(ts);
	}

	for (auto massPoints : m_MassPoints)
	{
		for (auto massPoint : massPoints)
		{
			massPoint->OnUpdate(ts);
		}
	}
}

void MGE::PhysicsObject_SoftBody::DrawSoftBody()
{
	for (auto massPoints : m_MassPoints)
	{
		for(auto massPoint : massPoints)
		{ 
			massPoint->DrawPhysicsObject();
		}
	}
}
