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

	float Stifiness = 1.0f;
	float RestLength = 1.0f;
	float DampFactor = 0.1f;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i == 0 && j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == width - 1 && j == height - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == 0 && j == height - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == width - 1 && j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == width - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));
				continue;
			}

			if (j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				continue;
			}

			if (j == height - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				continue;
			}

			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, 1.414f * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));

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
