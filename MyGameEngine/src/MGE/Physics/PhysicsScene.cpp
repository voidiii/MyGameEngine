#include "MGEpch.h"
#include "PhysicsScene.h"

namespace MGE {

	PhysicsScene::PhysicsScene(float height, float width, int numberOfObjects)
		: m_SceneHeight(height), m_SceneWidth(width), m_NumberOfObjects(numberOfObjects)
	{
		for (int i = 0; i < m_NumberOfObjects; i++)
		{
			CreateObjects();
		}
		GridManage();
	}

	void PhysicsScene::CreateObjects()
	{
		m_PhysicsObjects.push_back(MGE::CreateRef<MGE::CirclePhyicsObject>(
			MGE::Vec2{ rand() / double(RAND_MAX) * 5.0f,
			rand() / double(RAND_MAX) * 5.0f },
			MGE::Vec4{ 0.2f, 0.3f, 0.8f, 1.0f }
		));
		m_PhysicsObjects.back()->SetMotionLimit(m_SceneWidth, m_SceneHeight);
	}

	void PhysicsScene::OnUpdate(Timestep ts)
	{
		FindCollisions();
		m_Grid.clear();
		GridManage();
	}

	void PhysicsScene::ElasticCollisions(Ref<MGE::CirclePhyicsObject> i, Ref<MGE::CirclePhyicsObject> j) 
	{
		Vec2 hit_distance = Vec2(i->GetPosition() - j->GetPosition());
		Vec2 hit_direction = mathter::Normalize(hit_distance);

		float temp_i = mathter::Dot(Vec2(i->GetVelocity() - j->GetVelocity()), hit_distance);
		float temp_j = mathter::Dot(Vec2(-i->GetVelocity() + j->GetVelocity()), -hit_distance);

		float mass_i = i->GetMass();
		float mass_j = j->GetMass();

		Vec2 v_i = i->GetVelocity();
		Vec2 v_j = j->GetVelocity();

		Vec2 delta_v = v_i - v_j;
		Vec2 delta_x = hit_distance;
		float delta_x_square = mathter::Length(hit_distance) * mathter::Length(hit_distance);

		Vec2 new_v_i = v_i - ((2.0f * mass_j) / (mass_i + mass_j)) * (temp_i) / (delta_x_square) * (hit_distance);
		Vec2 new_v_j = v_j - ((2.0f * mass_i) / (mass_i + mass_j)) * (temp_j) / (delta_x_square) * (-hit_distance);

		i->ChangeVelocity(new_v_i);
		j->ChangeVelocity(new_v_j);

		i->UpdatePosition((0.2f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
		j->UpdatePosition(-(0.2f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
	}

	void PhysicsScene::FindCollisions()
	{
		for (int i = 1; i < (int)(m_SceneHeight * 2) - 1; i++)
		{
			for (int j = 1; j < (int)(m_SceneWidth * 2) - 1; j++)
			{
				for (auto k : m_Grid[i][j]->GridObjects)
				{
					for(int a = -1; a <= 1; a ++)
					{
						for(int b = -1; b <= 1; b++)
						{
							for (auto k_1 = m_Grid[i + a][j + b]->GridObjects.begin(); k_1 != m_Grid[i + a][j + b]->GridObjects.end(); k_1++)
							{
								if(k == *k_1) 
									continue;

								Vec2 hit_distance = Vec2(k->GetPosition() - (*k_1)->GetPosition());
								if (mathter::Length(hit_distance) < 0.2f)
								{
									ElasticCollisions(k, (*k_1));
								}
							}
						}
					}
				}
			}
		}
		
		//for (int i = 0; i < m_NumberOfObjects; i++)
		//{
		//	for (int j = i + 1; j < m_NumberOfObjects; j++)
		//	{
		//		Vec2 hit_distance = Vec2(m_PhysicsObjects[i]->GetPosition() - m_PhysicsObjects[j]->GetPosition());
		//		if (mathter::Length(hit_distance) < 0.2f)
		//		{
		//			ElasticCollisions(m_PhysicsObjects[i], m_PhysicsObjects[j]);
		//		}
		//	}
		//}
	}

	void PhysicsScene::GridManage()
	{
		for (int i = -(int)m_SceneHeight; i < (int)m_SceneHeight; i++)
		{
			std::vector<Ref<Grid>> temp;
			for (int j = -(int)m_SceneWidth; j < (int)m_SceneWidth; j++)
			{
				temp.push_back(CreateRef<Grid>(
					j, i
				));
			}
			m_Grid.push_back(temp);
		}

		for(auto object : m_PhysicsObjects)
		{
			int x = (int)(object->GetPosition().x + m_SceneWidth);
			int y = (int)(object->GetPosition().y + m_SceneHeight);

			m_Grid[y][x]->GridObjects.push_back(object);
		}

	}

}