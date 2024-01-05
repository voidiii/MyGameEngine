#include "MGEpch.h"
#include "PhysicsObject_SoftBody.h"

MGE::PhysicsObject_SoftBody::PhysicsObject_SoftBody(int width, int height)
{
	m_MassPoints.reserve(width * height);
	for(int i = 0; i < height; i++)
	{
		std::vector<Ref<MassPoint>> temp;
		for(int j = 0; j < width; j++)
		{
			temp.push_back(CreateRef<MassPoint>(MGE::Vec2_Physics(i * 1.5, j * 1.5 + 5), i * i + j));
		}
		m_MassPoints.push_back(temp);
	}

	float Stifiness = 5.0f;
	float RestLength = 1.5f;
	float DampFactor = 0.5f;
	float coifficient = 1.4f;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 && j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == height - 1 && j == width - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == 0 && j == width - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == height - 1 && j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
				continue;
			}

			if (i == height - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));
				continue;
			}

			if (j == 0)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				continue;
			}

			if (j == width - 1)
			{
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
				m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
				continue;
			}

			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j + 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i][j - 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j - 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j + 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i + 1][j - 1], m_MassPoints[i][j]));
			m_Springs.push_back(CreateRef<Spring>(Stifiness, coifficient * RestLength, DampFactor, i * i + j, m_MassPoints[i - 1][j + 1], m_MassPoints[i][j]));

		}
	}
}

void MGE::PhysicsObject_SoftBody::OnUpdate(Timestep ts)
{
	//ResolveSlefCollision();
	
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
	ResolveSlefCollision();
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

void MGE::PhysicsObject_SoftBody::ResolveSlefCollision()
{
	for (auto massPoints_i : m_MassPoints)
	{
		for (auto massPoint_i : massPoints_i)
		{
			for (auto massPoints_j : m_MassPoints)
			{
				for (auto massPoint_j : massPoints_j)
				{
					if (massPoint_j == massPoint_i)
					{
						continue;
					}

					float distance = glm::length(massPoint_i->GetPosition() - massPoint_j->GetPosition());
					Vec2_Physics direction = glm::normalize(massPoint_i->GetPosition() - massPoint_j->GetPosition());

					if (distance < massPoint_i->GetDiameter())
					{
						massPoint_i->UpdatePosition((massPoint_i->GetDiameter() - distance) * direction / 2.0f);
						massPoint_j->UpdatePosition( -(massPoint_j->GetDiameter() - distance) * direction / 2.0f);
						massPoint_i->ChangeVelocity(1.0f * glm::dot(direction, massPoint_i->GetVelocity()) * direction + massPoint_i->GetVelocity());
						massPoint_j->ChangeVelocity(-1.0f * glm::dot( direction, massPoint_j->GetVelocity()) * direction + massPoint_j->GetVelocity());
					}
				}
			}
		}
	}
}
